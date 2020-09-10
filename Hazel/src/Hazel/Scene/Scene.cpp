#include "hzpch.h"
#include "Scene.h"

#include "Component.h"
#include "Entity.h"
#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hazel {


	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(this, m_registry.create());

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		entity.AddComponent<TransformComponent>();
		
		return entity;
	}

}
