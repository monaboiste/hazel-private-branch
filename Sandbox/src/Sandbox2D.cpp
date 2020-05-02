#include <Hazel.h>

#include "imgui\imgui.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Sandbox2D.h"



Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_bricksTexture = Hazel::Texture2D::Create("assets/textures/bricks.jpg");

	m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };
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
		Hazel::RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		Hazel::Renderer2D::ResetStats();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 60.0f;

		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());

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
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	bool showConsole = true;
	Hazel::ImGuiConsole::OnImGuiRender(&showConsole);
	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
	ImGui::Text("Quad Count:  %d", stats.QuadCount);
	ImGui::Text("Vertices:    %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices:     %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);
}
