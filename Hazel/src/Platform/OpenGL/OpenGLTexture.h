#pragma once

#include "Hazel\Renderer\Texture.h"

#include <glad\glad.h>

#include <string>
#include <cstdint>

namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() override { return m_width; }
		virtual uint32_t GetHeight() override { return m_height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_rendererID == ((OpenGLTexture2D&)other).m_rendererID;
		}
	
	private:
		uint32_t m_rendererID;
		uint32_t m_width, m_height;
		GLenum m_internalFormat;
		GLenum m_dataFormat;
		std::string m_path;
	};

}