#pragma once

#include "Eagle/Core/Layers/Layer.h"

namespace Egl {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();
		void LetEventsThrough(bool letThrogh) { mLetMouseThrough = letThrogh; }

		void SetDarkThemeColors();
	private:
		bool mLetMouseThrough = true;
		float mTime = 0.0f;
	};
}
