#pragma once

#include "hzpch.h"

#include "Core.h"
#include "Events\Event.h"

namespace Hazel {

	struct WindowProps
	{
		std::string windowTitle;
		unsigned int screenWidth;
		unsigned int screenHeight;

		WindowProps(const std::string& title = "Hazel Engine", unsigned int width = 1280, unsigned int height = 720)
			:windowTitle(title), screenWidth(width), screenHeight(height)
		{}
	};

	// Interface representing a desktop system based Window
	class HAZEL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}