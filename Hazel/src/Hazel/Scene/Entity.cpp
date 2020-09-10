#include "hzpch.h"
#include "Entity.h"

namespace Hazel {

	Entity::Entity(Scene* scene, entt::entity entityHandle)
		: m_scene(scene), m_entityHandle(entityHandle)
	{
	}

}
