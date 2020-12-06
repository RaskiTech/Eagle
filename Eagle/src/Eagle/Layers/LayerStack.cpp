#include "EaglePCH.h"
#include "LayerStack.h"

namespace Egl {
	LayerStack::LayerStack() {
		mLayerInsert = mLayers.begin();
	}
	LayerStack::~LayerStack() {
		for (Layer* layer : mLayers)
			delete layer;
	}

	void LayerStack::AddLayer(Layer* layer) {
		mLayerInsert = mLayers.emplace(mLayerInsert, layer);
	}
	void LayerStack::RemoveLayer(Layer* layer) {
		auto foundLayer = std::find(mLayers.begin(), mLayers.end(), layer);
		if (foundLayer != mLayers.end()) {
			mLayers.erase(foundLayer);
			mLayerInsert--;
		}
	}
	void LayerStack::AddOverlay(Layer* layer) {
		mLayers.emplace_back(layer);
	}
	void LayerStack::RemoveOverlay(Layer* layer) {
		auto foundLayer = std::find(mLayers.begin(), mLayers.end(), layer);
		if (foundLayer != mLayers.end())
			mLayers.erase(foundLayer);
	}
}