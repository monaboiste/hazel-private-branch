#include "hzpch.h"
#include "Scene.h"

#include "Component.h"
#include "Entity.h"
#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hazel {


	Scene::Scene(const std::string& name)
		: m_name(name)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			m_registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& scriptComponent) {
				
				// @TODO: Move to Scene::OnPlay()
				if (scriptComponent.Instance == nullptr)
				{
					scriptComponent.Instance = scriptComponent.InstantiateScriptFunc();
					scriptComponent.Instance->m_entity = Entity(this, entity);

					scriptComponent.Instance->OnCreate();
				}
				scriptComponent.Instance->OnUpdate(ts);
			});
		}

		// Render 2D Scene
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = m_registry.view<TransformComponent, CameraComponent>();

		for (auto entity : view)
		{
			auto [tc, cc] = view.get<TransformComponent, CameraComponent>(entity);
			if (cc.Primary)
			{
				mainCamera = &cc.Camera;
				cameraTransform = tc.GetTransform();
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [tc, sc] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(tc.GetTransform(), sc.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [tc, sc] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(tc.GetTransform(), sc.Color);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		// Resize Non-Fixed Aspect Ratio cameras
		auto view = m_registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(this, m_registry.create());

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	Entity Scene::GetPrimaryActiveCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity(const_cast<Scene*>(this), entity);
		}
		return {};
	}

	// ------
	template <typename T>
	void Scene::OnComponentAdded(Entity, T& component)
	{
		HZ_CORE_ASSERT(false, "Default specialization is forbidden!");
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity, TagComponent& component) {}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity, TransformComponent& component) {}

	template <>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity, SpriteRendererComponent& component) {}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
	}

	template <>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity, NativeScriptComponent& component) {}
}
