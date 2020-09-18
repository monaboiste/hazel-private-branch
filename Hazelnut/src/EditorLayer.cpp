#include <Hazel.h>

#include "imgui\imgui.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "EditorLayer.h"

namespace Hazel {

	static constexpr uint32_t s_mapWidth = 24;
	static constexpr char* s_mapTiles = {
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWDDWWWWWWWWWW"
		"WWWWWWWWDDDDDDDDDWWWWWWW"
		"WWWWWWDDDDDDDDDDDDWWWWWW"
		"WWWWWDDDDDDWWDDDDDDWWWWW"
		"WWWWDDDDDDDWWDDDDDDDWWWW"
		"WWWDDDDDDDDDDDDDDDDDDWWW"
		"WWWWWWWWDDDDDDDDDDDDWWWW"
		"WWWDDDDDDDDDDDDDDDDDWWWW"
		"WWWWDDDDDDDDDDDDDDDDWWWW"
		"WWWWWDDDDDDDDDDDDDDWWWWW"
		"WWWWWWDDDDDDDDDDDDWWWWWW"
		"WWWWWWWWDDDDDDDDDWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
	};


	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1080;
		fbSpec.Height = 720;
		m_frameBuffer = FrameBuffer::Create(fbSpec);

		m_bricksTexture = Texture2D::Create("assets/textures/bricks.jpg");

		m_cameraController.SetZoomLevel(5.0f);

		m_spriteSheet = Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
		m_textureStairs = SubTexture2D::CreateFromCoords(m_spriteSheet, { 7.0f, 6.0f }, { 128.0f, 128.0f });
		m_textureTree = SubTexture2D::CreateFromCoords(m_spriteSheet, { 2.0f, 1.0f }, { 128.0f, 128.0f }, { 1.0f, 2.0f });

		m_mapWidth = s_mapWidth;
		m_mapHeight = (uint32_t)strlen(s_mapTiles) / s_mapWidth;

		m_textureMap['W'] = SubTexture2D::CreateFromCoords(m_spriteSheet, { 11, 11 }, { 128, 128 });
		m_textureMap['D'] = SubTexture2D::CreateFromCoords(m_spriteSheet, { 6, 11 }, { 128, 128 });

		// Scene
		m_activeScene = CreateRef<Scene>();

		m_squareEntt = m_activeScene->CreateEntity("Square");
		m_squareEntt.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.3f, 0.0f, 1.0f));

		m_mainCameraEntt = m_activeScene->CreateEntity("Main Camera");
		m_mainCameraEntt.AddComponent<CameraComponent>().Primary = true;

		m_secondCameraEntt = m_activeScene->CreateEntity("Second Camera");
		m_secondCameraEntt.AddComponent<CameraComponent>();
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
			m_cameraController.OnResize(m_viewportSize.x, m_viewportSize.y);

			m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if (m_viewportFocused)
			m_cameraController.OnUpdate(ts);

		m_frameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		Renderer2D::ResetStats();

		//for (uint32_t y = 0; y < m_mapHeight; y++)
		//{
		//	for (uint32_t x = 0; x < m_mapWidth; x++)
		//	{
		//		char tileType = s_mapTiles[x + y * m_mapWidth];
		//		Ref<SubTexture2D> subtex;

		//		if (m_textureMap.find(tileType) != m_textureMap.end())
		//			subtex = m_textureMap[tileType];
		//		else // invalid tile type
		//			subtex = m_textureStairs;

		//		// draw around origin
		//		float centerX = x - m_mapWidth / 2.0f;
		//		float centerY = m_mapHeight / 2.0f - y;		// flip coords, tiles render from the bottom
		//		Renderer2D::DrawQuad({ centerX, centerY }, { 1, 1 }, subtex);
		//	}
		//}

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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

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

		if (m_squareEntt)
		{
			auto& sprite = m_squareEntt.GetComponent<SpriteRendererComponent>();
			auto& squareName = m_squareEntt.GetComponent<TagComponent>().Tag + " color";

			ImGui::Begin(squareName.data());
			ImGui::ColorPicker4("", glm::value_ptr(sprite.Color));
			ImGui::End();
		}

		ImGui::Begin("Camera Switch Test");
		{
			ImGui::DragFloat2("Main Camera Transform", glm::value_ptr(m_mainCameraEntt.GetComponent<TransformComponent>().Transform[3]));

			if (ImGui::Checkbox("Main Camera", &m_primaryCamera))
			{
				m_mainCameraEntt.GetComponent<CameraComponent>().Primary = m_primaryCamera;
				m_secondCameraEntt.GetComponent<CameraComponent>().Primary = !m_primaryCamera;
			}
		}
		{
			auto& secondCamera = m_mainCameraEntt.GetComponent<CameraComponent>().Camera;
			float orthoSize = secondCamera.GetOrthographicSize();

			if (ImGui::DragFloat("Second Camera Orthosize", &orthoSize))
				secondCamera.SetOrthographicSize(orthoSize);
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused();
		Application::Get().GetImGuiLayer()->SetBlockImGuiEvents(!m_viewportFocused);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_frameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((ImTextureID)(intptr_t)textureID, { m_viewportSize.x, m_viewportSize.y }, { 0, 1 }, { 1, 0 }); // Image is flipped.

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_cameraController.OnEvent(e);
	}

}