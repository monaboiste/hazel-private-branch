#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform\OpenGL\OpenGLRendererAPI.h"

namespace Hazel {

	RendererAPI* RenderCommand::ms_RendererAPI = new OpenGLRendererAPI;

}