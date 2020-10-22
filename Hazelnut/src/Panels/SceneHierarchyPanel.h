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
		void DrawComponent(const std::string& label, Entity entity, ComponentSpecific drawUIFunc = nullptr)
		{
			if (entity.HasComponent<T>())
			{

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

				bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.data());

				ImGui::SameLine(ImGui::GetWindowWidth() - 45.0f);
				if (ImGui::Button("+", { 20, 20 }))
					ImGui::OpenPopup("ComponentSettings");

				bool deleteComponent = false;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove Component"))
						deleteComponent = true;

					ImGui::EndPopup();
				}
				ImGui::PopStyleVar();

				if (open)
				{
					if (drawUIFunc != nullptr)
						drawUIFunc();
				}

				if (deleteComponent)
					entity.RemoveComponent<T>();

				ImGui::TreePop();
			}
		}

	private:
		Ref<Scene> m_context;
		Entity m_selectionContext;
	};

}