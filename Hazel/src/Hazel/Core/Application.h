#pragma once

#include "Hazel\Core\Window.h"
#include "Hazel\Events\ApplicationEvent.h"
#include "Hazel\Events\Event.h"
#include "Hazel\Core\LayerStack.h"
#include "Hazel\ImGui\ImGuiLayer.h"
#include "Hazel\Core\Timestep.h"

namespace Hazel {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_window; }
		static inline Application& Get() { return *ms_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		static Application* ms_instance;
		bool m_running = true;
		bool m_minimized = false;
		std::unique_ptr<Window> m_window;
		LayerStack m_layerStack;
		ImGuiLayer* m_imguiLayer;	
		float m_lastFrameTime = 0.0f;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}