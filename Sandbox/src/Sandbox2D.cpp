#include <Hazel.h>

#include "imgui\imgui.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Sandbox2D.h"

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


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	Hazel::FrameBufferSpecification fbSpec;
	fbSpec.Width = 1080;
	fbSpec.Height = 720;
	m_frameBuffer = Hazel::FrameBuffer::Create(fbSpec);

	m_bricksTexture = Hazel::Texture2D::Create("assets/textures/bricks.jpg");

	m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };

	m_cameraController.SetZoomLevel(5.0f);

	m_spriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
	m_textureStairs = Hazel::SubTexture2D::CreateFromCoords(m_spriteSheet, { 7.0f, 6.0f }, { 128.0f, 128.0f });
	m_textureTree = Hazel::SubTexture2D::CreateFromCoords(m_spriteSheet, { 2.0f, 1.0f }, { 128.0f, 128.0f }, { 1.0f, 2.0f });

	m_mapWidth = s_mapWidth;
	m_mapHeight = (uint32_t)strlen(s_mapTiles) / s_mapWidth;

	m_textureMap['W'] = Hazel::SubTexture2D::CreateFromCoords(m_spriteSheet, { 11, 11 }, { 128, 128 });
	m_textureMap['D'] = Hazel::SubTexture2D::CreateFromCoords(m_spriteSheet, { 6, 11 }, { 128, 128 });
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	// Update
	{
		m_cameraController.OnUpdate(ts);
	}

	// Render
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		m_frameBuffer->Bind();
		Hazel::RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		Hazel::Renderer2D::ResetStats();
	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
#if 0
		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());

		static float rotation = 0.0f;
		rotation += ts * 60.0f;
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Hazel::Renderer2D::DrawRotatedQuad({ 0.5f, 0.8f, 1.0f }, { 1.0f, 0.8f }, glm::radians(45.0f), m_squareColor);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 1.0f, 0.5f }, { 1.8f, 2.2f }, { 0.0f, 0.8f, 0.0f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.1f }, { 1.8f, 2.2f }, m_bricksTexture, 10.0f, { 0.8f, 0.1f, 0.2f, 0.6f });
		Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, -1.0f, 0.0f }, { 0.8f, 0.5f }, glm::radians(rotation), m_bricksTexture);

		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.8f };
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}

		}
		Hazel::Renderer2D::EndScene();

#endif

		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Hazel::Input::GetMousePosition();
			auto width = Hazel::Application::Get().GetWindow().GetWidth();
			auto height = Hazel::Application::Get().GetWindow().GetHeight();

			auto bounds = m_cameraController.GetBounds();
			auto pos = m_cameraController.GetCamera().GetPosition();
			auto angle = glm::radians(m_cameraController.GetCamera().GetRotation());

			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();

			// translate position, pivot point is camera position
			float particleX = x * glm::cos(angle) - y * glm::sin(angle);
			float particleY = x * glm::sin(angle) + y * glm::cos(angle);

			particleX += pos.x;
			particleY += pos.y;

			m_particle.Position = { particleX,  particleY };

			for (int i = 0; i < 5; i++)
				m_particleSystem.Emit(m_particle);
		}

		m_particleSystem.OnUpdate(ts);
		m_particleSystem.OnRender(m_cameraController.GetCamera());


		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());

		for (uint32_t y = 0; y < m_mapHeight; y++)
		{
			for (uint32_t x = 0; x < m_mapWidth; x++)
			{
				char tileType = s_mapTiles[x + y * m_mapWidth];
				Hazel::Ref<Hazel::SubTexture2D> subtex;

				if (m_textureMap.find(tileType) != m_textureMap.end())
					subtex = m_textureMap[tileType];
				else // invalid tile type
					subtex = m_textureStairs;

				// draw around origin
				float centerX = x - m_mapWidth / 2.0f;
				float centerY = m_mapHeight / 2.0f - y;		// flip coords, tiles render from the bottom
				Hazel::Renderer2D::DrawQuad({ centerX, centerY }, { 1, 1 }, subtex);
			}
		}
		Hazel::Renderer2D::EndScene();
		m_frameBuffer->Unbind();
	}
}

void Sandbox2D::OnImGuiRender()
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
				Hazel::Application::Get().Close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// static bool showConsole = true;
	// Hazel::ImGuiConsole::OnImGuiRender(&showConsole);
	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
	ImGui::Text("Quad Count:  %d", stats.QuadCount);
	ImGui::Text("Vertices:    %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices:     %d", stats.GetTotalIndexCount());

	uint32_t textureID = m_frameBuffer->GetColorAttachmentRendererID();
	ImGui::Image((ImTextureID)textureID, { 1080.0f, 720.0f }, { 0, 1 }, { 1, 0 });

	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);
}
