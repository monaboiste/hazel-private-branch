#include "SceneHierarchyPanel.h"
#include "Hazel/Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		{
			m_context->m_registry.each([this](auto entityID) {
				Entity entity(m_context.get(), entityID);
				DrawEntityNode(entity);
				});
		}

		// Clear properties window when clicked blank space
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		{
			if (m_selectionContext)
				DrawComponents(m_selectionContext);
		}
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

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		DrawComponent<TagComponent>("Tag", entity, [&entity]() {
			std::string& tag = entity.GetComponent<TagComponent>().Tag;

			static constexpr size_t BUFF_SIZE = 256;
			static char buffer[BUFF_SIZE];

			std::memset(buffer, 0, BUFF_SIZE);
			strcpy_s(buffer, BUFF_SIZE, tag.data());

			if (ImGui::InputText("Tag", buffer, BUFF_SIZE - 1))
				tag = std::string(buffer);
		});

		DrawComponent<TransformComponent>("Transform", entity, [&entity]() {
			auto& transform = entity.GetComponent<TransformComponent>().Transform;
			ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.25f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [&entity]() {
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			ImGui::Checkbox("Primary", &cameraComponent.Primary);

			// Matches ProjectionType enum in SceneCamera class
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int type = 0; type < 2; type++)
				{
					bool selected = (currentProjectionTypeString == projectionTypeStrings[type]);
					if (ImGui::Selectable(projectionTypeStrings[type], &selected))
					{
						currentProjectionTypeString = projectionTypeStrings[type];
						camera.SetProjectionType((SceneCamera::ProjectionType)type);
					}

					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float fov = glm::degrees(camera.GetPerspectiveVertivalFOV());
				if (ImGui::DragFloat("Field of view", &fov, 1.05f))
					camera.SetPerspectiveVertivalFOV(glm::radians(fov));

				float nearClip = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near clip", &nearClip, 0.25f))
					camera.SetPerspectiveNearClip(nearClip);

				float farClip = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far clip", &farClip, 5.0f))
					camera.SetPerspectiveFarClip(farClip);
			}
			else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &size, 0.25f))
					camera.SetOrthographicSize(size);

				float nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near clip", &nearClip, 0.25f))
					camera.SetOrthographicNearClip(nearClip);

				float farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far clip", &farClip, 0.25f))
					camera.SetOrthographicFarClip(farClip);

				ImGui::Checkbox("Fixed aspect ratio", &cameraComponent.FixedAspectRatio);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Render", entity, [&entity]() {
			auto& src = entity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("", glm::value_ptr(src.Color));
		});
	}
}
