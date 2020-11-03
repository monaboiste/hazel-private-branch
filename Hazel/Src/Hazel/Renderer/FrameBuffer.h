#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false; // Unbind framebuffers and render to the screen.
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};

}