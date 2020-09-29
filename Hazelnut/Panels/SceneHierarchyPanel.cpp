#include "SceneHierarchyPanel.h"
#include "Hazel/Scene/Component.h"

#include <imgui/imgui.h>

namespace Hazel {
	
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_context->m_registry.each([this](auto entityID) {
			Entity entity(m_context.get(), entityID);
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_context = context;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_OpenOnArrow : 0) | ImGuiTreeNodeFlags_Selected;
		
		bool expanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.data());
		
		if (ImGui::IsItemClicked())
			m_selectionContext = entity;

		if (expanded)
		{
			ImGui::Text("Property 1");
			ImGui::Text("Property 2");
			ImGui::Text("Property 3");
			ImGui::TreePop();
		}
	}

}
