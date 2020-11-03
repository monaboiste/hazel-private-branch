#pragma once
#include "Hazel/Core/Core.h"
#include "Scene.h"

namespace Hazel {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		bool Serialize(const std::string& filepath);			// serializes to text - yaml
		bool SerializeRuntime(const std::string& filepath);		// serializes to binary

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_scene;
	};

}