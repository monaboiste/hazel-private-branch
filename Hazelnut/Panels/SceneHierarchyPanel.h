#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"

#include <functional>
#include <imgui/imgui.h>


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
		void DrawComponents(Entity entity);

		using ComponentSpecific = std::function<void()>;

		template<typename T>
		void DrawComponent(const std::string& label, Entity entity, ComponentSpecific drawUIFunc)
		{
			if (entity.HasComponent<T>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, label.data()))
				{
					if (drawUIFunc != nullptr)
						drawUIFunc();
				}
				ImGui::TreePop();
			}
		}

	private:
		Ref<Scene> m_context;
		Entity m_selectionContext;
	};

}