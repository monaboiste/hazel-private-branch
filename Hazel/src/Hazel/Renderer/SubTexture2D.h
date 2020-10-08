#pragma once

#include <glm/glm.hpp>

#include <Hazel/Renderer/Texture.h>

namespace Hazel {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture() const { return m_texture; }
		const glm::vec2* GetTexCoords() const { return m_texCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = glm::vec2(1.0f, 1.0f));
	
	private:
		Ref<Texture2D> m_texture;
		glm::vec2 m_texCoords[4];
	};

}