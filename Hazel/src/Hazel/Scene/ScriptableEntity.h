#pragma once

#include "Entity.h"

namespace Hazel {

	class ScriptableEntity : public Entity
	{
		friend class Scene;
	public:

		template <typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

	private:
		Entity m_entity;
	};

}