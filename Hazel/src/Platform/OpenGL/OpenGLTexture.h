#pragma once

#include "Hazel\Renderer\Texture.h"
#include <string>
#include <cstdint>

namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() override { return m_width; }
		virtual uint32_t GetHeight() override { return m_height; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		uint32_t m_rendererID;
		uint32_t m_width, m_height;
		std::string m_path;
	};

}