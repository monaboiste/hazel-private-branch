#pragma once

#include "RendererAPI.h"

namespace Hazel {

	class RenderCommand
	{
	public:
		inline static void Clear()
		{
			ms_RendererAPI->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			ms_RendererAPI->SetClearColor(color);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) 
		{
			ms_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* ms_RendererAPI;
	};

}