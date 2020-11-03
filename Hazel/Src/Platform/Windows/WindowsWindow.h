#pragma once

#include "Hazel\Core\Window.h"
#include "Hazel\Renderer\GraphicsContext.h"

#include <GLFW\glfw3.h>


namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_data.width; }
		inline unsigned int GetHeight() const override { return m_data.height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_window; }
	
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	
	private:
		GLFWwindow* m_window;
		Scope<GraphicsContext> m_context;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};
}