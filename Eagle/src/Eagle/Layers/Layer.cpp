#include "EaglePCH.h"
#include "Layer.h"

namespace Egl {
	Layer::Layer(const std::string& layerName, bool isActive)
		: mName(layerName), mActive(isActive) {}

	Layer::~Layer() {}
}