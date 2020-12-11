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

		Entity GetSelectedEntity() const { return m_selectionContext; }
		
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& label, Entity entity, UIFunction drawUIFunc)
		{
			if (entity.HasComponent<T>())
			{
				auto& component = entity.GetComponent<T>();

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | 
					ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | 
					ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				float available = ImGui::GetContentRegionAvailWidth();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImGui::Separator();
				bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.data());
				ImGui::PopStyleVar();

				ImGui::SameLine(available - lineHeight * 0.5f);
				if (ImGui::Button("...", { lineHeight, lineHeight }))
					ImGui::OpenPopup("ComponentSettings");

				bool deleteComponent = false;

				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove Component"))
						deleteComponent = true;

					ImGui::EndPopup();
				}

				if (open)
					drawUIFunc(component);

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