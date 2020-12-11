#include "hzpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Component.h"

#include <yaml-cpp/yaml.h>

namespace YAML {
	// YAML expect those for deserialization

	template <>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (node.IsSequence() && node.size() == 3)
			{
				rhs.x = node[0].as<float>();
				rhs.y = node[1].as<float>();
				rhs.z = node[2].as<float>();
				return true;
			}
			return false;
		}
	};

	template <>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (node.IsSequence() && node.size() == 4)
			{
				rhs.x = node[0].as<float>();
				rhs.y = node[1].as<float>();
				rhs.z = node[2].as<float>();
				rhs.w = node[3].as<float>();
				return true;
			}
			return false;
		}
	};
}

namespace Hazel {

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_scene(scene)
	{
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;	// @TODO: Add unique entity id here when implemented

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			
			out << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<CameraComponent>())
		{
			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (uint32_t)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << camera.GetPerspectiveVertivalFOV();
			out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& src = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

	bool SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_scene->m_name.data();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_scene->m_registry.each([&](auto entityID)
		{
			Entity entity(m_scene.get(), entityID);
			if (!entity)
				return;
			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);

		if (fout.good())
		{
			fout << out.c_str();
			return true;
		}

		return false;
	}

	bool SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented.
		HZ_CORE_ASSERT(false, "");
		return false;
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream fin(filepath);
		if (fin.good())
		{
			std::stringstream ss;
			ss << fin.rdbuf();

			YAML::Node data = YAML::Load(ss.str());
			if (!data["Scene"])
				return false;

			std::string sceneName = data["Scene"].as<std::string>();
			HZ_CORE_TRACE("Deserializing scene \"{0}\"", sceneName);

			auto entities = data["Entities"];
			if (entities)
			{
				for (auto entity : entities)
				{
					uint64_t uuid = entity["Entity"].as<uint64_t>(); // @TODO

					std::string tag;
					auto tagComponent = entity["TagComponent"];
					if (tagComponent)
						tag = tagComponent["Tag"].as<std::string>();

					HZ_CORE_TRACE("Deserializing entity ID: \"{0}\", name: \"{1}\"", uuid, tag);

					Entity deserializedEntity = m_scene->CreateEntity(tag);
					
					auto transformComponent = entity["TransformComponent"];
					if (transformComponent)
					{
						// Entities always have transform component
						auto& tc = deserializedEntity.GetComponent<TransformComponent>();
						tc.Translation = transformComponent["Translation"].as<glm::vec3>();
						tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
						tc.Scale = transformComponent["Scale"].as<glm::vec3>();
					}

					auto cameraComponent = entity["CameraComponent"];
					if (cameraComponent)
					{
						auto& camera = cameraComponent["Camera"];
						auto& cc = deserializedEntity.AddComponent<CameraComponent>();
						uint32_t projectionType = camera["ProjectionType"].as<uint32_t>();
						cc.Camera.SetProjectionType((SceneCamera::ProjectionType)projectionType);
						float fov = camera["PerspectiveVerticalFOV"].as<float>();
						cc.Camera.SetPerspectiveVertivalFOV(fov);
						float perspertiveNearClip = camera["PerspectiveNearClip"].as<float>();
						cc.Camera.SetPerspectiveNearClip(perspertiveNearClip);
						float perspertiveFarClip = camera["PerspectiveFarClip"].as<float>();
						cc.Camera.SetPerspectiveFarClip(perspertiveFarClip);
						float orthoSize = camera["OrthographicSize"].as<float>();
						cc.Camera.SetOrthographicSize(orthoSize);
						float orthoNearClip = camera["OrthographicNearClip"].as<float>();
						cc.Camera.SetOrthographicNearClip(orthoNearClip);
						float orthoFarClip = camera["OrthographicFarClip"].as<float>();
						cc.Camera.SetOrthographicFarClip(orthoFarClip);
					}

					auto srComponent = entity["SpriteRendererComponent"];
					if (srComponent)
					{
						deserializedEntity.AddComponent<SpriteRendererComponent>();
						auto& src = deserializedEntity.GetComponent<SpriteRendererComponent>();
						src.Color = srComponent["Color"].as<glm::vec4>();
					}
				}
			}
			return true;
		}
		return false; 
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented.
		HZ_CORE_ASSERT(false, "");
		return false;
	}

}
