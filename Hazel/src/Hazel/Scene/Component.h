#pragma once

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Hazel {


	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::scale(glm::mat4(1.0f), Scale)
				* rotation;
		}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
		TagComponent(const TagComponent&) = default;
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		
		ScriptableEntity* (*InstantiateScriptFunc)() = nullptr;
		void (*DestroyScriptFunc)(NativeScriptComponent*) = nullptr;

		template <typename T>
		void Bind()
		{
			InstantiateScriptFunc = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScriptFunc = [](NativeScriptComponent* scriptComponent) { delete scriptComponent->Instance; scriptComponent->Instance = nullptr; };
		}
	};
}