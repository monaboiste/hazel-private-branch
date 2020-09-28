#pragma once

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>

namespace Hazel {


	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}
		TransformComponent(const TransformComponent&) = default;

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
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
		
		std::function<void()> InstantiateFunc = nullptr;
		std::function<void()> DestroyInstanceFunc = nullptr;
		std::function<void(ScriptableEntity*)> OnCreateFunc = nullptr;
		std::function<void(ScriptableEntity*)> OnDestroyFunc = nullptr;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunc = nullptr;

		template <typename T>
		void Bind()
		{
			InstantiateFunc = [&]() { Instance = new T(); };
			DestroyInstanceFunc = [&]() { delete ((T*)Instance); Instance = nullptr; };
			OnCreateFunc = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunc = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunc = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
		}
	};
}