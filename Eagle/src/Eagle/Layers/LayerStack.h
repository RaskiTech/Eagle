#pragma once
#include "EaglePCH.h"
#include "Layer.h"

namespace Egl {
	class EAGLE_API LayerStack {
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
		std::vector<Layer*>::iterator mLayerInsert;
	};
}
