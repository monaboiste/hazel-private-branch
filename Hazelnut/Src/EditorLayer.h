#pragma once
#include <Hazel.h>

#include "Panels/SceneHierarchyPanel.h"

namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		Ref<FrameBuffer> m_frameBuffer;

		glm::vec2 m_viewportSize = { 1080.0f, 720.0f };

		Ref<Scene> m_activeScene;
		Entity m_orangeSquareEntt;
		Entity m_greenSquareEntt;
		Entity m_mainCameraEntt;
		Entity m_secondCameraEntt;
		bool m_primaryCamera = true;

		// Panels
		SceneHierarchyPanel m_scenePanel;
	};

}