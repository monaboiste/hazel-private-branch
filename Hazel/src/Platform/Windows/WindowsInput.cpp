#include "hzpch.h"
#include "Hazel\Core\Input.h"

#include "Hazel\Core\Application.h"

#include <GLFW\glfw3.h>

namespace Hazel {

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		/*auto[xpos, ypos] = GetMousePosition();
		return xpos;*/
		return GetMousePosition().first;
	}

	float Input::GetMouseY()
	{
		/*auto[xpos, ypos] = GetMousePosition();
		return ypos;*/
		return GetMousePosition().second;
	}

}