#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel\Core\Input.h"
#include "Hazel\Core\KeyCodes.h"

namespace Hazel
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio)
		, m_rotation(rotation)
		, m_bounds({ -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel })
		, m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(Key::A))
		{
			m_cameraPosition.x -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
			m_cameraPosition.y -= sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			m_cameraPosition.x += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
			m_cameraPosition.y += sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(Key::W))
		{
			m_cameraPosition.x += -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
			m_cameraPosition.y += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			m_cameraPosition.x -= -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
			m_cameraPosition.y -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * ts;
		}

		if (m_rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_cameraRotation += m_cameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(Key::E))
				m_cameraRotation -= m_cameraRotationSpeed * ts;

			m_camera.SetRotation(m_cameraRotation);
		}

		m_camera.SetPosition(m_cameraPosition);

		m_cameraTranslationSpeed = m_zoomLevel;
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_aspectRatio = width / height;
		RecalculateView();
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		m_zoomLevel -= e.GetYOffset() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		RecalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		m_aspectRatio = (float)e.GetWidth() / e.GetHeight();
		RecalculateView();
		return false;
	}

	void OrthographicCameraController::RecalculateView()
	{
		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
		m_camera.SetProjection(m_bounds.Left, m_bounds.Right, m_bounds.Bottom, m_bounds.Top);
	}

}
