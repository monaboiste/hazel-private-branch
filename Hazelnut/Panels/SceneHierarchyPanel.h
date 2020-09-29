#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"


namespace Hazel {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void OnImguiRender();
		void SetContext(const Ref<Scene>& context);
		
	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> m_context;
		Entity m_selectionContext;
	};

}