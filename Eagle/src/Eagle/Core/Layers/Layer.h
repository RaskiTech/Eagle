#pragma once
#include <string>

#include "Eagle/Events/Event.h"


namespace Egl {
	class Layer {
	public:
		Layer(const std::string& layerName = "Layer", bool isActive = true);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline bool IsActive() const { return mActive; }
		inline void SetActive(bool active) { mActive = active; }
		inline const std::string& GetName() const { return mName; }
	protected:
		std::string mName;
		bool mActive;
	};
}