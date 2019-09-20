#include <Hazel.h>
#include "imgui\imgui.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Platform\OpenGL\OpenGLShader.h"


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_cameraController(1280.0f / 720.0f, true)
	{
		m_vertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_position" },
			{ Hazel::ShaderDataType::Float4, "a_color" }
		};
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2 };

		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_vertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		m_squareVA.reset(Hazel::VertexArray::Create());
		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout(
			{
				{ Hazel::ShaderDataType::Float3, "a_position" },
				{ Hazel::ShaderDataType::Float2, "a_texCoord" }
			});
		m_squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0 };

		Hazel::Ref<Hazel::IndexBuffer> squareIB;
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

		m_shader = Hazel::Shader::Create("VertexPosColor", vertexSource, fragmentSource);

		std::string flatColorVertexSource = R"(
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

		std::string flatColorFragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;
			uniform vec4 u_color;

			void main()
			{
				color = u_color;
			}
		)";

		m_flatColorShader = Hazel::Shader::Create("FlatColor", flatColorVertexSource, flatColorFragmentSource);


		auto textureShader = m_shaderLibrary.Load("assets/shaders/Texture.glsl");
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();

		m_bricksTexture = Hazel::Texture2D::Create("assets/textures/bricks.jpg");
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_texture", 0);

		m_colorSplashTexture = Hazel::Texture2D::Create("assets/textures/color-splash.png");
	}


	void OnUpdate(Hazel::Timestep ts) override
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

		auto textureShader = m_shaderLibrary.Get("Texture");

		m_bricksTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_colorSplashTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// render triangle
		// Hazel::Renderer::Submit(m_shader, m_vertexArray);

		Hazel::Renderer::EndScene();
		// ----------------------------------
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_squareColor));

		ImGui::End();
	}

	void OnEvent(Hazel::Event& e) override
	{
		m_cameraController.OnEvent(e);
	}
	
private:
	Hazel::ShaderLibrary m_shaderLibrary;
	Hazel::Ref<Hazel::VertexArray> m_vertexArray;
	Hazel::Ref<Hazel::Shader> m_shader;
	Hazel::Ref<Hazel::VertexArray> m_squareVA;
	Hazel::Ref<Hazel::Shader> m_flatColorShader;
	Hazel::Ref<Hazel::Texture2D> m_bricksTexture;
	Hazel::Ref<Hazel::Texture2D> m_colorSplashTexture;

	Hazel::OrthographicCameraController m_cameraController;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
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