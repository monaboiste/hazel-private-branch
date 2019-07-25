#include <Hazel.h>
#include "imgui\imgui.h"
#include <glm\gtc\matrix_transform.hpp>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f, 0.0f, 0.0f)
	{
		m_vertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_position" },
			{ Hazel::ShaderDataType::Float4, "a_color" }
		};
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2 };

		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_vertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		m_squareVA.reset(Hazel::VertexArray::Create());
		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout(
			{
				{ Hazel::ShaderDataType::Float3, "a_position" },
			});
		m_squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		m_squareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;
			
			uniform mat4 u_viewProjection;			
			uniform mat4 u_transform;			

			out vec3 v_position;
			out vec4 v_color;
			
			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);	
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;

			void main()
			{
				color = vec4(v_position * 0.5 + 0.5, 1.0);
				color = v_color;
			}
		)";

		m_shader.reset(new Hazel::Shader(vertexSource, fragmentSource));

		std::string squareVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			
			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;
			
			out vec3 v_position;
			
			void main()
			{
				v_position = a_position;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);	
			}
		)";

		std::string squareFragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;

			void main()
			{
				color = vec4(1.0, 0.0, 0.0, 1.0);
			}
		)";

		m_redShader.reset(new Hazel::Shader(squareVertexSource, squareFragmentSource));
	}


	void OnUpdate(Hazel::Timestep timestep) override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_cameraPosition.x += m_cameraMoveSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_cameraPosition.x -= m_cameraMoveSpeed * timestep;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_cameraPosition.y -= m_cameraMoveSpeed * timestep;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_cameraPosition.y += m_cameraMoveSpeed * timestep;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation -= m_cameraRotationSpeed;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation += m_cameraRotationSpeed;


		Hazel::RenderCommand::SetClearColor({ 0.15f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 sqPos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), sqPos) * scale;
				Hazel::Renderer::Submit(m_redShader, m_squareVA, transform);

			}
		}

		Hazel::Renderer::Submit(m_shader, m_vertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Hazel::Event& event) override
	{
		
	}
	
private:
	std::shared_ptr<Hazel::VertexArray> m_vertexArray;
	std::shared_ptr<Hazel::Shader> m_shader;
	std::shared_ptr<Hazel::VertexArray> m_squareVA;
	std::shared_ptr<Hazel::Shader> m_redShader;

	Hazel::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraMoveSpeed = 0.5f;
	float m_CameraRotation = 0.0f;
	float m_cameraRotationSpeed = 2.0f;
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}