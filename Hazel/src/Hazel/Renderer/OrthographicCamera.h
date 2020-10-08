#pragma once

#include <glm\glm.hpp>

namespace Hazel {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera() = default;

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec3& position) { m_position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_rotation; }
		void SetRotation(float rotation) {	m_rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
	
	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;
	};
}