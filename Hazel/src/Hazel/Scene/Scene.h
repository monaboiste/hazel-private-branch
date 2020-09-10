#pragma once

#include <entt.h>

#include "Hazel/Core/Timestep.h"

namespace Hazel {


	class Scene
	{
		friend class Entity;
	public:
		Scene();
		~Scene();
		
		void OnUpdate(Timestep ts);

		Entity CreateEntity(const std::string& name = "");
	private:
		entt::registry m_registry;
	};


}