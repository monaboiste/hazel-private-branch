#include "hzpch.h"
#include "EditorCamera.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazel {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip)
		, Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		glm::vec2 mouse = { Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
		m_initialMousePosition = mouse;

		if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			MousePan(delta);
		if ((Input::IsMouseButtonPressed(Mouse::ButtonLeft)) && (Input::IsKeyPressed(Key::LeftAlt)))
			MouseRotate(delta);
		//if ((Input::IsMouseButtonPressed(Mouse::ButtonRight)) && (Input::IsKeyPressed(Key::LeftAlt)))
		//	MouseZoom(delta.y);
		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}

	void EditorCamera::UpdateProjection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_yaw = m_pitch = 0.0f; // Lock the camera's rotation
		m_position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_view = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
		m_view = glm::inverse(m_view);
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		//auto [xSpeed, ySpeed] = PanSpeed();
		float xSpeed = 0.2f;
		float ySpeed = 0.2f;
		m_focalPoint += -GetRightDirection() * delta.x * xSpeed * m_distance;
		m_focalPoint += GetUpDirection() * delta.y * ySpeed * m_distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = (GetUpDirection().y < 0 ? -1.0f : 1.0f);
		m_yaw += yawSign * delta.x * RotationSpeed();
		m_pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_distance -= delta * ZoomSpeed();
		if (m_distance < 1.0f)
		{
			m_focalPoint += GetForwardDirection();
			m_distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_focalPoint - GetForwardDirection() * m_distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		// copied from graph plotting webside
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_distance * 0.5f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max distance = 100.0f
		return speed;
	}

}
