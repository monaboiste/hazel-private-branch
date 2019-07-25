#include "hzpch.h"
#include "OpenGLContext.h"

#include "Hazel\Log.h"

#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_windowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize GLAD!");

		HZ_CORE_INFO("  Renderer:\t{0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Version:\t{0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}

