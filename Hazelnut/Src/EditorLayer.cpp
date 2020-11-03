#include <imgui\imgui.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "EditorLayer.h"

namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1080;
		fbSpec.Height = 720;
		m_frameBuffer = FrameBuffer::Create(fbSpec);

		// Scene
		m_activeScene = CreateRef<Scene>();

		m_orangeSquareEntt = m_activeScene->CreateEntity("Square");
		m_orangeSquareEntt.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.3f, 0.0f, 1.0f));

		m_greenSquareEntt = m_activeScene->CreateEntity("Square2");
		m_greenSquareEntt.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 1.0f, 0.1f, 1.0f));
		m_greenSquareEntt.GetComponent<TransformComponent>().Translation += glm::vec3(3.0f, 0.5f, 0.0f);

		m_secondCameraEntt = m_activeScene->CreateEntity("Second Camera");
		m_secondCameraEntt.AddComponent<CameraComponent>();

		m_mainCameraEntt = m_activeScene->CreateEntity("Main Camera");
		m_mainCameraEntt.AddComponent<CameraComponent>().Primary = true;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate() {}
			void OnDestroy() {}
			void OnUpdate(Timestep ts)
			{
				glm::vec3& translation = GetComponent<TransformComponent>().Translation;
				const float kSpeed = 5.0f;

				if (Input::IsKeyPressed(HZ_KEY_A))
					translation.x -= kSpeed * ts;
				else if (Input::IsKeyPressed(HZ_KEY_D))
					translation.x += kSpeed * ts;

				if (Input::IsKeyPressed(HZ_KEY_W))
					translation.y += kSpeed * ts;
				else if (Input::IsKeyPressed(HZ_KEY_S))
					translation.y -= kSpeed * ts;
			}
		};

		m_mainCameraEntt.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_scenePanel.SetContext(m_activeScene);

		SceneSerializer serializer(m_activeScene);
		serializer.Serialize("Assets/Scenes/scene.hazel");
		//serializer.Dserialize("Assets/Scenes/scene.hazel");
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();

		// Resize
		if (FrameBufferSpecification spec = m_frameBuffer->GetSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&	// zero sized framebuffer is invalid
			(spec.Width != (uint32_t)m_viewportSize.x || spec.Height != (uint32_t)m_viewportSize.y))
		{
			m_frameBuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		m_frameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		Renderer2D::ResetStats();
		m_activeScene->OnUpdate(ts);
		m_frameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		static bool showDockspace = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Level Editor", &showDockspace, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		
		ImGuiStyle& style = ImGui::GetStyle();
		float defaultMinWidth = style.WindowMinSize.x;
		style.WindowMinSize.x = 385.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = defaultMinWidth;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		static bool showConsole = true;
		ImGuiConsole::OnImGuiRender(&showConsole);
		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
		ImGui::Text("Quad Count:  %d", stats.QuadCount);
		ImGui::Text("Vertices:    %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices:     %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_frameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((ImTextureID)(intptr_t)textureID, { m_viewportSize.x, m_viewportSize.y }, { 0, 1 }, { 1, 0 }); // Image is flipped.

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

		// Scene Hierarchy Panel
		m_scenePanel.OnImguiRender();
	}

	void EditorLayer::OnEvent(Event& e)
	{
	}

}