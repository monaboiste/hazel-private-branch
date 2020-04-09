#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {


	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		/* @TODO: texid, masks ... */
	};

	struct Renderer2DData
	{
		const uint32_t			MaxQuads = 10000;
		const uint32_t			MaxVertices = MaxQuads * 4;
		const uint32_t			MaxIndices = MaxQuads * 6;
		static const uint32_t	MaxTexSlots = 32;		/* @TODO: RenderCaps */

		uint32_t				QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTexSlots> TexSlots;
		uint32_t				TexSlotIndex = 1;		// Texture Slot 0 is White Texture

		Ref<VertexArray>		QuadVertexArray;
		Ref<VertexBuffer>		QuadVertexBuffer;
		Ref<Shader>				QuadShader;
		Ref<Texture2D>			WhiteTexture;

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DData s_data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_data.QuadVertexArray = VertexArray::Create();

		s_data.QuadVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex));
		s_data.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_position" },
				{ ShaderDataType::Float4, "a_color" },
				{ ShaderDataType::Float2, "a_texCoord" },
				{ ShaderDataType::Float, "a_texIndex" },
				{ ShaderDataType::Float, "a_tilingFactor" }
			});

		s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);

		s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices];

		// Generate indices. Indices are uploaded to GPU immediately
		uint32_t* quadIndices = new uint32_t[s_data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_data.MaxIndices);
		s_data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		delete[] quadIndices;

		s_data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create samplers2D
		int32_t samplers[s_data.MaxTexSlots];
		for (int32_t i = 0; i < s_data.MaxTexSlots; i++)
			samplers[i] = i;

		s_data.QuadShader = Shader::Create("assets/shaders/Texture.glsl");
		s_data.QuadShader->Bind();
		s_data.QuadShader->SetIntArray("u_textures", samplers, s_data.MaxTexSlots);

		// Set index slot 0 to white texture
		s_data.TexSlots[0] = s_data.WhiteTexture;

		s_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		s_data.QuadShader->Bind();
		s_data.QuadShader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());

		s_data.QuadIndexCount = 0;
		s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

		s_data.TexSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_data.QuadVertexBufferPtr - (uint8_t*)s_data.QuadVertexBufferBase;
		s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		HZ_PROFILE_FUNCTION();

		// Bind textures
		for (uint32_t i = 0; i < s_data.TexSlotIndex; i++)
			s_data.TexSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		const float texIndex = 0.0f; // white texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		// bottom left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// bottom right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TexSlotIndex; i++)
		{
			if (*s_data.TexSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_data.TexSlotIndex;
			s_data.TexSlots[s_data.TexSlotIndex] = texture;
			s_data.TexSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		// bottom left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// bottom right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		const float texIndex = 0.0f; // white texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		// bottom left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// bottom right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TexSlotIndex; i++)
		{
			if (*s_data.TexSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_data.TexSlotIndex;
			s_data.TexSlots[s_data.TexSlotIndex] = texture;
			s_data.TexSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		// bottom left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// bottom right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 0.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top right
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(1.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;
		// top left
		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = tintColor;
		s_data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 1.0f);
		s_data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;
	}

}

