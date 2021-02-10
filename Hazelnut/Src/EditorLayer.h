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
		bool OnKeyPressed(KeyPressedEvent& e);
		
		// Dialogs
		void NewScene();
		void OpenSceneFile();
		void SaveSceneFileAs();

	private:
		Ref<FrameBuffer> m_frameBuffer;

		glm::vec2 m_viewportSize = { 1080.0f, 720.0f };
		bool m_viewportFocused = true;
		bool m_viewportHovered = true;

		Ref<Scene> m_activeScene;
		EditorCamera m_editorCamera;
		Entity m_orangeSquareEntt;
		Entity m_greenSquareEntt;
		Entity m_mainCameraEntt;
		Entity m_secondCameraEntt;
		bool m_primaryCamera = true;
		int m_gizmoType = -1;

		glm::vec2 m_viewportBounds[2];

		// Panels
		SceneHierarchyPanel m_scenePanel;
	};

}