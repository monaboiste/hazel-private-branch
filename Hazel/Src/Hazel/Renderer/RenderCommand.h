#pragma once

#include "Hazel\Renderer\RendererAPI.h"

namespace Hazel {

	class RenderCommand
	{
	public:
		static void Init()
		{
			ms_RendererAPI->Init();
		}

		inline static void Clear()
		{
			ms_RendererAPI->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			ms_RendererAPI->SetClearColor(color);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) 
		{
			ms_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			ms_RendererAPI->SetViewport(x, y, width, height);
		}

	private:
		static Scope<RendererAPI> ms_RendererAPI;
	};

}