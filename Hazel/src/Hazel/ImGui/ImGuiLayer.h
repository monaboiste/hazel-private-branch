#pragma once

#include "Hazel\Layer.h"
#include "Hazel\Events\ApplicationEvent.h"
#include "Hazel\Events\KeyEvent.h"
#include "Hazel\Events\MouseEvent.h"

namespace Hazel {

	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		void virtual OnAttach() override;
		void virtual OnDetach() override; 
		virtual void OnImGuiRender() override;
	
		void Begin();
		void End();
	private:
		float m_time = 0.0f;
	};

}