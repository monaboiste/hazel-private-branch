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
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	{
		m_cameraController.OnUpdate(ts);
	}

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Hazel::Renderer2D::DrawRotatedQuad({ 0.5f, 0.8f }, { 1.0f, 0.8f }, glm::radians(45.0f), m_squareColor);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 1.8f, 2.2f }, glm::radians(45.0f), m_bricksTexture, 10.0f, glm::vec4(0.0f, 0.8f, 0.0f, 1.0f));

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	bool showConsole = true;
	Hazel::ImGuiConsole::OnImGuiRender(&showConsole);
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);
}
