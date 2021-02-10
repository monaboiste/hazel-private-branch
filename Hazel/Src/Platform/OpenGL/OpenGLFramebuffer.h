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
		virtual int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_spec; }
		void Invalidate();

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override 
		{ 
			HZ_CORE_ASSERT(index < m_colorAttachmentIDs.size(), "Out od bounds");
			return m_colorAttachmentIDs[index];
		}
	
	private:
		uint32_t m_rendererID = 0;
		FrameBufferSpecification m_spec;

		std::vector<FramebuffetTextureSpecification> m_colorAttachmentSpecs;
		FramebuffetTextureSpecification m_depthAttachmentSpec;

		std::vector<uint32_t> m_colorAttachmentIDs;
		uint32_t m_depthAttachmentID;
	};

}