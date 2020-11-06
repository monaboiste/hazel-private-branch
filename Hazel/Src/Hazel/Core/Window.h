#pragma once

#include "hzpch.h"

#include "Hazel\Events\Event.h"

namespace Hazel {

	struct WindowProps
	{
		std::string windowTitle;
		uint32_t screenWidth;
		uint32_t screenHeight;

		WindowProps(const std::string& title = "Hazel Engine", uint32_t width = 1280, uint32_t height = 720)
			: windowTitle(title), screenWidth(width), screenHeight(height) {}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}