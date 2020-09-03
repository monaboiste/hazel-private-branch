#pragma once

#include "Hazel\Core\Core.h"

namespace Hazel {

	// Implementation based on OS - found in Platform folder
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};

}