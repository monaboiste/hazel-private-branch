#include "hzpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Hazel {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_spec(spec)
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);	
		glDeleteTextures(1, &m_colorAttachment);
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_rendererID != 0)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(1, &m_colorAttachment);
			glDeleteTextures(1, &m_depthAttachment);
		}
		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		glCreateTextures(GL_TEXTURE_2D ,1, &m_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.Width, m_spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_spec.Width, m_spec.Height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);
		
		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_spec.Width, m_spec.Height);
	}
	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_spec.Width = width;
		m_spec.Height = height;

		Invalidate();
	}
}