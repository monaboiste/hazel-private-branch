#include "hzpch.h"
#include "Hazel\Log.h"
#include "OpenGLTexture.h"
#include <glad\glad.h>
#include "stb_image.h"

Hazel::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	: m_path(path)
{
	stbi_set_flip_vertically_on_load(1);

	int width, height, channels;
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	HZ_CORE_ASSERT(data, "Failed to load image!");

	m_width = width;
	m_height = height;

	GLenum format = GL_RGB;
	if (channels == 4)
		format = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, GL_RGB8, m_width, m_height);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Hazel::OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_rendererID);
}

void Hazel::OpenGLTexture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_rendererID);
}
