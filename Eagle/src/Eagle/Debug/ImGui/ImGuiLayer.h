#pragma once

namespace Egl {
	class ImGuiLayer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnEvent(Event& event);

		void Begin();
		void End();
		void LetEventsThrough(bool letThrogh) { mLetMouseThrough = letThrogh; }

		void SetDarkThemeColors();
	private:
		bool mLetMouseThrough = true;
		float mTime = 0.0f;
	};
}
