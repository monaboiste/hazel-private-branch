#pragma once

#include <entt.h>

#include "Hazel/Core/Timestep.h"

namespace Hazel {


	class Scene
	{
	public:
		Scene();
		~Scene();
		
		void OnUpdate(Timestep ts);

		// TEMPORARY
		entt::entity CreateEntity();
		entt::registry& GetRegistery() { return m_enttRegistery; }

	private:
		entt::registry m_enttRegistery;
	};


}