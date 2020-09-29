#pragma once

#include "Scene.h"

#include "Hazel/Core/Log.h"
#include <entt.h>

namespace Hazel {

	class Entity
	{
	public:
		Entity() = default;
		Entity(Scene* scene, entt::entity entityHandle);
		Entity(const Entity&) = default;
	
		template <typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.has<T>(m_entityHandle);
		}

		template <typename T>
		T& GetComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have required component!");
			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template <typename T, typename... Args>
		T& AddComponent(Args... args)
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		void RemoveComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have required component!");
			m_scene->m_registry.remove<T>(m_entityHandle);
		}

		operator bool() const { return m_entityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_entityHandle; }
		bool operator ==(const Entity& other) const 
		{ 
			return (m_entityHandle == other.m_entityHandle) && (m_scene == other.m_scene); 
		}

		bool operator !=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		Scene* m_scene = nullptr; // @TODO: switch to intrusive reference counting
		entt::entity m_entityHandle = entt::null;
	};

}