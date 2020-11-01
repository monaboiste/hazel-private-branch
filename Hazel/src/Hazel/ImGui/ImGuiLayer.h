#pragma once

#include "Hazel\Core\Layer.h"
#include "Hazel\Events\ApplicationEvent.h"
#include "Hazel\Events\KeyEvent.h"
#include "Hazel\Events\MouseEvent.h"
#include "Hazel\ImGui\ImGuiConsole.h"

namespace Hazel {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		void virtual OnAttach() override;
		void virtual OnDetach() override;
		void virtual OnEvent(Event& e) override;

		void SetBlockImGuiEvents(bool block) { m_blockImGuiEvent = block; }

		void Begin();
		void End();

	private:
		void SetDarkTheme();

	private:
		float m_time = 0.0f;
		bool m_blockImGuiEvent = true;
	};

}