#include <Hazel.h>

#include "imgui\imgui.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Platform\OpenGL\OpenGLShader.h"
#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
{	
}

void Sandbox2D::OnAttach()
{
	
}

void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	m_cameraController.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();


	// ----------------------------------
	Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	/* @TODO Add Shader::SetMat4, Shader::SetFloat4 */


	Hazel::Renderer2D::DrawQuad({ 0.5f, 0.8f }, { 1.0f, 0.8f }, { 0.1f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.5f, 0.6f, 0.3f, 1.0f });

	Hazel::Renderer2D::EndScene();
	// ----------------------------------
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);
}
