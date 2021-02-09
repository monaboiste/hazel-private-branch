#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	enum class FramebufferTextureFormat
	{
		NONE = 0,
		// Color
		RGBA8,
		// Depth/Stencil
		DEPTH24STENCIL8,

		DEPTH = DEPTH24STENCIL8
	};

	struct FramebuffetTextureSpecification
	{
		FramebuffetTextureSpecification() = default;
		FramebuffetTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format)
		{
		}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::NONE;
		// @TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebuffetTextureSpecification> attachments)
			: Attachments(attachments)
		{
		}

		std::vector<FramebuffetTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;
		FramebufferAttachmentSpecification AttachmentSpec;

		bool SwapChainTarget = false; // Unbind framebuffers and render to the screen.
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};

}