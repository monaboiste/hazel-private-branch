#pragma once

#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_spec; }
		void Invalidate();

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_colorAttachment; }

		virtual void Bind() override;
		virtual void Unbind() override;
	private:
		uint32_t m_rendererID;
		uint32_t m_colorAttachment, m_depthAttachment;
		FrameBufferSpecification m_spec;
	};

}