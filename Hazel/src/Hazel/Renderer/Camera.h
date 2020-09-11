#pragma once

#include <glm/glm.hpp>

namespace Hazel {

	// Runtime Camera Class
	class Camera
	{
	public:
		Camera(const glm::mat4& projection)
			: m_projection(projection) {}

		const glm::mat4& GetProjection() const { return m_projection; }
	private:
		glm::mat4 m_projection = glm::mat4(1.0f);
	};

}