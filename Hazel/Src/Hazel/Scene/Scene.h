#pragma once

#include <entt.h>

#include "Hazel/Core/Timestep.h"

namespace Hazel {


	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;
	public:
		Scene();
		~Scene();
		
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = "");
		void DestroyEntity(Entity entity);
	
	private:
		template <typename T>
		void OnComponentAdded(Entity, T& component);

	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	};


}