#pragma once

#include "Hazel\Core.h"
#include "Layer.h"

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlayer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlayer);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};

}