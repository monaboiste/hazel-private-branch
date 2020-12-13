#pragma once

#include "Hazel/Renderer/Camera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		float GetDistance() const { return m_distance; }
		void SetDistance(float distance) { m_distance = distance; }

		void SetViewportSize(float width, float height) { m_viewportWidth = width; m_viewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetView() const { return m_view; }
		glm::mat4 GetViewProjection() const { return m_projection * m_view; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::vec3 GetRightDirection() const;
		const glm::vec3& GetPosition() const { return m_position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		float m_fov = 45.0f;
		float m_aspectRatio = 1.778f;
		float m_nearClip = 0.1f;
		float m_farClip = 1000.0f;

		float m_viewportWidth = 1280.0f;
		float m_viewportHeight = 720.0f;

		float m_distance = 10.0f;
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;

		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_focalPoint = { 0.0f, 0.0f, 10.0f };
		glm::vec2 m_initialMousePosition = { 0.0f, 0.0f };
	};

}