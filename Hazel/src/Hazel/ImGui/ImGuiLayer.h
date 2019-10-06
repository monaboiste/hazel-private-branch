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
		ImGuiLayer(bool showDemo = false, bool showConsole = true);
		~ImGuiLayer() = default;

		void virtual OnAttach() override;
		void virtual OnDetach() override; 
		virtual void OnImGuiRender() override;
	
		void Begin();
		void End();
	private:
		float m_time = 0.0f;
		bool m_showDemo;
		bool m_showConsole;
	};

}