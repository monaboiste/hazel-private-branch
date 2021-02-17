#include <imgui\imgui.h>
#include <ImGuizmo\ImGuizmo.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Hazel\Math\Math.h"

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
		fbSpec.AttachmentSpec = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::DEPTH };
		m_frameBuffer = FrameBuffer::Create(fbSpec);

#if 0
		// Scene
		m_activeScene = CreateRef<Scene>();
		m_orangeSquareEntt = m_activeScene->CreateEntity("Square");
		m_orangeSquareEntt.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.3f, 0.0f, 1.0f));

		m_greenSquareEntt = m_activeScene->CreateEntity("Square2");
		m_greenSquareEntt.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 1.0f, 0.1f, 1.0f));
		m_greenSquareEntt.GetComponent<TransformComponent>().Translation += glm::vec3(3.0f, 0.5f, 0.0f);

		m_secondCameraEntt = m_activeScene->CreateEntity("Second Camera");
		m_secondCameraEntt.AddComponent<CameraComponent>().Primary = false;

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

				if (Input::IsKeyPressed(Key::A))
					translation.x -= kSpeed * ts;
				else if (Input::IsKeyPressed(Key::D))
					translation.x += kSpeed * ts;

				if (Input::IsKeyPressed(Key::W))
					translation.y += kSpeed * ts;
				else if (Input::IsKeyPressed(Key::S))
					translation.y -= kSpeed * ts;
			}
		};

		m_mainCameraEntt.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#else
		m_activeScene = CreateRef<Scene>();
		m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_scenePanel.SetContext(m_activeScene);

		m_editorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		m_editorCamera.SetViewportSize(m_viewportSize.x, m_viewportSize.y);

		SceneSerializer serializer(m_activeScene);
		serializer.Deserialize("Assets/Scenes/fake-cube.hazel");
#endif
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
			m_editorCamera.SetViewportSize(m_viewportSize.x, m_viewportSize.y);
		}

		m_frameBuffer->Bind();
		// Render
		RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		Renderer2D::ResetStats();

		// Clear entity ID attachment to -1
		m_frameBuffer->ClearColorAttachment(1, -1);

		// Update
		if (m_viewportFocused)
			m_editorCamera.OnUpdate(ts);
		// Update Scene
		m_activeScene->OnUpdateEditor(ts, m_editorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;

		// Flip to match texture (bottom <0,0>)
		my = m_viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseX <= (int)m_viewportSize.x &&
			mouseY >= 0 && mouseY <= (int)m_viewportSize.y)
		{
			int pixelData = m_frameBuffer->ReadPixel(1, mouseX, mouseY);
			HZ_WARN("pixel: {0}", pixelData);
		}

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
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenSceneFile();
				if (ImGui::MenuItem("Save As...", "Ctrl+S"))
					SaveSceneFileAs();
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

		ImVec2 viewportOffset = ImGui::GetCursorPos(); // includes tab bar

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockImGuiEvents(!m_viewportFocused && !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_frameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((ImTextureID)(intptr_t)textureID, { m_viewportSize.x, m_viewportSize.y }, { 0, 1 }, { 1, 0 }); // Image is flipped.

		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + m_viewportSize.x, minBound.y + m_viewportSize.y };
		m_viewportBounds[0] = { minBound.x, minBound.y };
		m_viewportBounds[1] = { maxBound.x, maxBound.y };

		// Gizmo
		Entity selectedEntity = m_scenePanel.GetSelectedEntity();
		if (selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Runtime Camera
			//auto cameraEntity = m_activeScene->GetPrimaryActiveCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor Camera
			const glm::mat4& cameraProjection = m_editorCamera.GetProjection();
			glm::mat4 cameraView = m_editorCamera.GetView();

			// Entity
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::A);
			float snapValue = (m_gizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f);

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
								 (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
								 nullptr, (snap ? snapValues : nullptr));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation = rotation;
				tc.Scale = scale;
			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

		// Scene Hierarchy Panel
		m_scenePanel.OnImguiRender();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));

		if (m_viewportFocused && m_viewportHovered)
			m_editorCamera.OnEvent(e);
	}


	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		bool controlHolded = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (controlHolded)
				NewScene();
		}; break;
		case Key::O:
		{
			if (controlHolded)
				OpenSceneFile();
		}; break;
		case Key::S:
		{
			if (controlHolded)
				SaveSceneFileAs();
		}; break;
		// Gizmo
		case Key::Escape:
		{
			m_gizmoType = -1;
		}; break;
		case Key::T:
		{
			m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}; break;
		case Key::R:
		{
			m_gizmoType = ImGuizmo::OPERATION::ROTATE;
		}; break;
		case Key::E:
		{
			m_gizmoType = ImGuizmo::OPERATION::SCALE;
		}; break;
		}
		return true;
	}

	void EditorLayer::NewScene()
	{
		m_activeScene = CreateRef<Scene>();
		m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_scenePanel.SetContext(m_activeScene);
	}

	void EditorLayer::OpenSceneFile()
	{
		auto filename = FileDialog::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (filename)
		{
			// @NOTE: Deserialized scene doesn't contain scripts binded to entities

			m_activeScene = CreateRef<Scene>();
			m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_scenePanel.SetContext(m_activeScene);
			SceneSerializer serializer(m_activeScene);
			serializer.Deserialize(*filename);
		}
	}

	void EditorLayer::SaveSceneFileAs()
	{
		auto filename = FileDialog::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (filename)
		{
			SceneSerializer serializer(m_activeScene);
			serializer.Serialize(*filename);
		}
	}

}