#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

Hazel::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	: m_path(path)
{
	HZ_PROFILE_FUNCTION();

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

	m_internalFormat = internalFormat;
	m_dataFormat = dataFormat;

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

Hazel::OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	: m_width(width), m_height(height)
{
	HZ_PROFILE_FUNCTION();

	m_internalFormat = GL_RGBA8;
	m_dataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Hazel::OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_rendererID);
}

void Hazel::OpenGLTexture2D::SetData(void* data, uint32_t size)
{
	uint32_t bytesPerPixel = m_dataFormat == GL_RGBA ? 4 : 3;
	HZ_CORE_ASSERT(size == m_width * m_height * bytesPerPixel, "Data must be entire texture!");
	glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void Hazel::OpenGLTexture2D::Bind(uint32_t slot) const
{
	HZ_PROFILE_FUNCTION();

	glBindTextureUnit(slot, m_rendererID);
}

void Hazel::OpenGLTexture2D::Unbind() const
{
	HZ_PROFILE_FUNCTION();

	glBindTexture(GL_TEXTURE_2D, 0);
}
