#pragma once
#include "EaglePCH.h"
#include "Layer.h"

namespace Egl {
	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void RemoveLayer(Layer* layer);
		void AddLayer(Layer* layer);
		void RemoveOverlay(Layer* layer);
		void AddOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
		std::vector<Layer*>::iterator end() { return mLayers.end(); }
	private:
		std::vector<Layer*> mLayers;
		unsigned int mLayerInsertIndex = 0;
	};
}
