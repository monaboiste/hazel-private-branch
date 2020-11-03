#pragma once

#include "Entity.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel {

	class ScriptableEntity : public Entity
	{
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;

		template <typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

	private:
		Entity m_entity;
	};

}