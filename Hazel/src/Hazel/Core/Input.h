#pragma once

#include "Hazel\Core\Core.h"

namespace Hazel {

	class Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(int keycode) { return ms_instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return ms_instance->IsMouseButtonPressedImpl(button); }
		inline static float GetMouseX() { return ms_instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return ms_instance->GetMouseYImpl(); }
		inline static std::pair<float,float> GetMousePosition() { return ms_instance->GetMousePositionImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Scope<Input> ms_instance;
	};

}