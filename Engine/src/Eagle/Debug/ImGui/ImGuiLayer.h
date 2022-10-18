#pragma once

// From british palette, flatuicolors
#define EDITOR_COLOR_WHITE1     ImVec4{ 245.0f / 255, 246.0f / 255, 250.0f / 255, 1.0f }
#define EDITOR_COLOR_WHITE2     ImVec4{ 220.0f / 255, 221.0f / 255, 225.0f / 255, 1.0f }

#define EDITOR_COLOR_VIOLET1    ImVec4{ 156.0f / 255 - 0.1f, 136.0f / 255 - 0.1f, 255.0f / 255 - 0.1f, 1.0f }
#define EDITOR_COLOR_VIOLET2    ImVec4{ 140.0f / 255 - 0.1f, 122.0f / 255 - 0.1f, 230.0f / 255 - 0.1f, 1.0f }

#define EDITOR_COLOR_LIGHTBLUE1 ImVec4{ 64.0f / 255, 115.0f / 255, 158.0f / 255, 1.0f }
#define EDITOR_COLOR_LIGHTBLUE2 ImVec4{ 72.0f / 255, 126.0f / 255, 176.0f / 255, 1.0f }

#define EDITOR_COLOR_DARKBLUE1  ImVec4{ 25.0f / 255, 42.0f / 255, 86.0f / 255, 1.0f }
#define EDITOR_COLOR_DARKBLUE2  ImVec4{ 39.0f / 255, 60.0f / 255, 117.0f / 255, 1.0f }

#define EDITOR_COLOR_LIGHTGRAY1 ImVec4{ 127.0f / 255, 143.0f / 255, 166.0f / 255, 1.0f }
#define EDITOR_COLOR_LIGHTGRAY2 ImVec4{ 113.0f / 255, 128.0f / 255, 147.0f / 255, 1.0f }

#define EDITOR_COLOR_DARKGRAY1  ImVec4{ 53.0f / 255+0.15f, 59.0f / 255+0.15f, 72.0f / 255+0.15f, 1.0f }
#define EDITOR_COLOR_DARKGRAY2  ImVec4{ 53.0f / 255+0.1f, 59.0f / 255 + 0.1f, 72.0f / 255 + 0.1f, 1.0f }
#define EDITOR_COLOR_DARKGRAY3  ImVec4{ 47.0f / 255, 54.0f / 255, 64.0f / 255, 1.0f }

#define EDITOR_COLOR_DARK       ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f }

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
		std::string iniFilePath = "../imgui.ini";
	};
}
