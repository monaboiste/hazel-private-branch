#pragma once

#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_spec; }
		void Invalidate();

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_colorAttachment; }
	
	private:
		uint32_t m_rendererID = 0;
		uint32_t m_colorAttachment = 0, m_depthAttachment = 0;
		FrameBufferSpecification m_spec;
	};

}