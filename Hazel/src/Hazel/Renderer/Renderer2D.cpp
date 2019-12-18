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
		Ref<Shader>			QuadShader;
		Ref<Texture2D>		WhiteTexture;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

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

		s_data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_data->QuadShader = Shader::Create("assets/shaders/Texture.glsl");
		s_data->QuadShader->Bind();
		s_data->QuadShader->SetInt("u_texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		s_data->QuadShader->Bind();
		s_data->QuadShader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		s_data->QuadShader->SetFloat("u_tilingFactor", 1.0f);
		s_data->QuadShader->SetFloat4("u_color", color);
		s_data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->QuadShader->SetMat4("u_transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		s_data->QuadShader->SetFloat("u_tilingFactor", tilingFactor);
		s_data->QuadShader->SetFloat4("u_color", tintColor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->QuadShader->SetMat4("u_transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		s_data->QuadShader->SetFloat("u_tilingFactor", 1.0f);
		s_data->QuadShader->SetFloat4("u_color", color);
		s_data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }
		);
		s_data->QuadShader->SetMat4("u_transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		s_data->QuadShader->SetFloat("u_tilingFactor", tilingFactor);
		s_data->QuadShader->SetFloat4("u_color", tintColor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }
		);
		s_data->QuadShader->SetMat4("u_transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_data->QuadVertexArray);
	}

}

