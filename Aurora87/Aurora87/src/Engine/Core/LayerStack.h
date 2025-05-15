#pragma once

#include "Layer.h"

#include <iostream>
#include <vector>
#include <memory>

namespace Engine
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(std::shared_ptr<Layer> layer);
		void PopLayer(Layer* layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void PopOverlay(Layer* overlay);

		std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<std::shared_ptr<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}