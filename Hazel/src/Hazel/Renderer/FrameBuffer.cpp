#include "hzpch.h"
#include "FrameBuffer.h"

#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hazel {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>(spec);
		default: break;
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}