#include "hzpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Hazel {

	static bool IsDepthFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
		}
		return false;
	}

	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void BindTexture(bool multisampled, uint32_t id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void CreateTextures(bool multisampled, uint32_t* outID, size_t count)
	{
		glCreateTextures(TextureTarget(multisampled), (GLsizei)count, outID);
	}

	static void AttachColorTexture(uint32_t id, int samples, GLenum format, GLenum internalFormat, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	static GLenum HazelTextureFormatToGL(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
		}
		HZ_CORE_ASSERT(false, "FramebufferTextureFormat not supported");
		return GL_NONE;
	}


	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_spec(spec)
	{
		for (auto& attachment : m_spec.AttachmentSpec.Attachments)
		{
			auto format = attachment.TextureFormat;
			if (!IsDepthFormat(format))
				m_colorAttachmentSpecs.emplace_back(format);
			else
				m_depthAttachmentSpec = format;
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures((GLsizei)m_colorAttachmentIDs.size(), m_colorAttachmentIDs.data());
		glDeleteTextures(1, &m_depthAttachmentID);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_rendererID != 0)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures((GLsizei)m_colorAttachmentIDs.size(), m_colorAttachmentIDs.data());
			glDeleteTextures(1, &m_depthAttachmentID);

			m_colorAttachmentIDs.clear();
			m_depthAttachmentID = 0;
		}
		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		// Attachmnets
		bool multisampled = m_spec.Samples > 1;
		if (m_colorAttachmentSpecs.size())
		{

			m_colorAttachmentIDs.resize(m_colorAttachmentSpecs.size());
			CreateTextures(multisampled, m_colorAttachmentIDs.data(), m_colorAttachmentIDs.size());

			for (size_t i = 0; i < m_colorAttachmentSpecs.size(); i++)
			{
				BindTexture(multisampled, m_colorAttachmentIDs[i]);
				switch (m_colorAttachmentSpecs[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					AttachColorTexture(m_colorAttachmentIDs[i], m_spec.Samples, GL_RGBA, GL_RGBA8,
									   m_spec.Width, m_spec.Height, (int)i); break;
				case FramebufferTextureFormat::RED_INTEGER:
					AttachColorTexture(m_colorAttachmentIDs[i], m_spec.Samples, GL_RED_INTEGER, GL_R32I,
									   m_spec.Width, m_spec.Height, (int)i); break;
				}
			}

		}

		if (m_depthAttachmentSpec.TextureFormat != FramebufferTextureFormat::NONE)
		{
			CreateTextures(multisampled, &m_depthAttachmentID, 1);
			BindTexture(multisampled, m_depthAttachmentID);
			switch (m_depthAttachmentSpec.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_depthAttachmentID, m_spec.Samples, GL_DEPTH24_STENCIL8, 
								   GL_DEPTH_STENCIL_ATTACHMENT, m_spec.Width, m_spec.Height); break;
			}
		}

		if (m_colorAttachmentIDs.size() > 1)
		{
			HZ_CORE_ASSERT(m_colorAttachmentIDs.size() < 4, "Hazel supports up to 4 color attachments at the moment");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_colorAttachmentIDs.size(), buffers);
		}
		else if (m_colorAttachmentIDs.empty())
		{
			// Only depth pass
			glDrawBuffer(GL_NONE);
		}

		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	
	void OpenGLFrameBuffer::ClearColorAttachment(uint32_t attachmentIndex, int value) const
	{
		HZ_CORE_ASSERT(m_colorAttachmentIDs.size() < 4, "Hazel supports up to 4 color attachments at the moment");
		auto& spec = m_colorAttachmentSpecs[attachmentIndex];

		glClearTexImage(m_colorAttachmentIDs[attachmentIndex], 0, HazelTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
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
	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
	{
		// Framebuffer must be binded!
		HZ_CORE_ASSERT(attachmentIndex < m_colorAttachmentIDs.size(), "Attachment Index out of bound!");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		uint32_t pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
}