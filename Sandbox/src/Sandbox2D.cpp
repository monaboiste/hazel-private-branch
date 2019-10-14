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
	float squareVertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	m_squareVA.reset(Hazel::VertexArray::Create());
	Hazel::Ref<Hazel::VertexBuffer> squareVB;
	squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_position" }
		});
	m_squareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0 };

	Hazel::Ref<Hazel::IndexBuffer> squareIB;
	squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

	m_squareVA->SetIndexBuffer(squareIB);

	m_flatColorShader = Hazel::Shader::Create("assets/shaders/FlatColorShader.glsl");
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
	Hazel::Renderer::BeginScene(m_cameraController.GetCamera());

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_flatColorShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_flatColorShader)->UploadUniformFloat4("u_color", m_squareColor);


	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 sqPos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), sqPos) * scale;
			Hazel::Renderer::Submit(m_flatColorShader, m_squareVA, transform);
		}
	}

	Hazel::Renderer::EndScene();
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
