#include "hzpch.h"
#include "OpenGLTexture.h"

#include "Hazel\Core\Log.h"

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

	GLenum internalFormat = 0;
	GLenum dataFormat = 0;
	switch (channels)
	{
	case 1: internalFormat = GL_R8;		dataFormat = GL_RED;  break;
	case 3: internalFormat = GL_RGB8;	dataFormat = GL_RGB;  break;
	case 4: internalFormat = GL_RGBA8;	dataFormat = GL_RGBA; break;
	}
	HZ_CORE_ASSERT(internalFormat & dataFormat, "Texture format not supported!");
	
	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

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
