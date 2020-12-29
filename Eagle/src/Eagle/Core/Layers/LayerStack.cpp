#include "EaglePCH.h"
#include "LayerStack.h"

namespace Egl {
	LayerStack::LayerStack() {

	}
	LayerStack::~LayerStack() {
		for (Layer* layer : mLayers) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::AddLayer(Layer* layer) {
		mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
		mLayerInsertIndex++;
	}
	void LayerStack::RemoveLayer(Layer* layer) {
		auto foundLayer = std::find(mLayers.begin(), mLayers.end(), layer);
		if (foundLayer != mLayers.end()) {
			mLayers.erase(foundLayer);
			mLayerInsertIndex--;
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