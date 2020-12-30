#pragma once

#include "Eagle/Core/Layers/Layer.h"

namespace Egl {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float mTime = 0.0f;
	};
}
