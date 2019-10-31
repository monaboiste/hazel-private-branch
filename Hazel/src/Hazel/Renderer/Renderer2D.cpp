#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	struct Renderer2DStorage
	{
		Ref<VertexArray>	QuadVertexArray;
		Ref<Shader>			FlatColorShader;
		Ref<Shader>			TextureShader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::Init()
	{
		s_data = new Renderer2DStorage;

		float squareVertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		s_data->QuadVertexArray = VertexArray::Create();

		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		quadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_position" },
				{ ShaderDataType::Float2, "a_texCoord" }
			});

		s_data->QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

		uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		
		s_data->QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
		s_data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		s_data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		s_data->TextureShader->Bind();
		s_data->TextureShader->SetInt("u_texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data->FlatColorShader->Bind();
		s_data->FlatColorShader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());

		s_data->TextureShader->Bind();
		s_data->TextureShader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_data->FlatColorShader->Bind();
		s_data->FlatColorShader->SetFloat4("u_color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation */ glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->FlatColorShader->SetMat4("u_transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) 
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_data->TextureShader->Bind();
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* rotation */ glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->TextureShader->SetMat4("u_transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->QuadVertexArray);
	}
}
