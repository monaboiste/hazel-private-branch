#include "hzpch.h"
#include "Application.h"

#include "Hazel\Core\Core.h"
#include "Hazel\Core\Input.h"
#include "Hazel\Renderer\Renderer.h"

#include <GLFW\glfw3.h>

namespace Hazel {

	Application* Application::ms_instance = nullptr;

	Application::Application(const std::string& name)
	{
		HZ_PROFILE_FUNCTION();
		HZ_CORE_ASSERT(!ms_instance, "Application already exist!");
		ms_instance = this;

		m_window = Window::Create(WindowProps(name));
		m_window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);
	}


	Application::~Application()
	{
		HZ_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();

		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();

		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		HZ_PROFILE_FUNCTION();

		while (m_running)
		{
			HZ_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized)
			{
				HZ_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_layerStack)
					layer->OnUpdate(timestep);
			}

			m_imguiLayer->Begin();
			{
				HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_layerStack)
					layer->OnImGuiRender();
			}
			m_imguiLayer->End();

			m_window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_running = false;
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}
		m_minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}