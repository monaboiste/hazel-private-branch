#pragma once

#include "Hazel\Events\Event.h"
#include "Hazel\Core\Timestep.h"

namespace Hazel {

	class Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		// Overriden in Sandbox APP
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnImGuiRender() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }

	protected:
		std::string m_debugName;
	};

}