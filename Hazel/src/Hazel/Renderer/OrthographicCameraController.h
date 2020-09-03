#pragma once

#include "Hazel\Renderer\OrthographicCamera.h"
#include "Hazel\Core\Timestep.h"
#include "Hazel\Events\ApplicationEvent.h"
#include "Hazel\Events\MouseEvent.h"

#include <glm\glm.hpp>


namespace Hazel {
	
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotattion = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_camera; }
		const OrthographicCamera& GetCamera() const { return m_camera; }

		void SetZoomLevel(float level) { m_zoomLevel = level; RecalculateView(); }
		inline const float GetZoomLevel(float level) const { return m_zoomLevel; }
		inline const OrthographicCameraBounds& GetBounds() const { return m_bounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void RecalculateView();
	private:
		bool m_rotation;
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;
		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		OrthographicCameraBounds m_bounds;
		OrthographicCamera m_camera;
		float m_cameraRotation = 0.0f; /* In degrees, in the anti-clockwise direction */
		float m_cameraTranslationSpeed = 1.0f;
		float m_cameraRotationSpeed = 100.0f;
	};

}