#pragma once

#include "Hazel/Renderer/Camera.h"

namespace Hazel {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		const ProjectionType GetProjectionType() const { return m_projectionType; }
		void SetProjectionType(ProjectionType type) { m_projectionType = type; RecalculateProjection(); }
			
		const float GetPerspectiveVertivalFOV() const { return m_perspectiveFOV; }
		void SetPerspectiveVertivalFOV(float fov) { m_perspectiveFOV = fov; RecalculateProjection(); }
		const float GetPerspectiveNearClip() const { return m_perspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_perspectiveNear = nearClip; RecalculateProjection(); }
		const float GetPerspectiveFarClip() const { return m_perspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_perspectiveFar = farClip; RecalculateProjection(); }

		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		const float GetOrthographicSize() const { return m_orthographicSize; }
		void SetOrthographicSize(float size) { m_orthographicSize = size; RecalculateProjection(); }
		const float GetOrthographicNearClip() const { return m_orthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_orthographicNear = nearClip; RecalculateProjection(); }
		const float GetOrthographicFarClip() const { return m_orthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_orthographicFar = farClip; RecalculateProjection(); }

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);
	
	private:
		void RecalculateProjection();
	
	private:
		ProjectionType m_projectionType = ProjectionType::Orthographic;
		
		float m_perspectiveFOV = glm::radians(45.0f);
		float m_perspectiveNear = 0.01f, m_perspectiveFar = 1000.0f;

		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f, m_orthographicFar = 1.0f;

		float m_aspectRatio = 1.0f;
	};

}