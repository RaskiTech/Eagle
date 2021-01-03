#pragma once

namespace Egl {
	class Input {
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMousePressed(int mouseButton);
		static float MousePosX() { auto [x, y] = MousePos(); return x; }
		static float MousePosY() { auto [x, y] = MousePos(); return y; }
		static std::pair<float, float> MousePos();
	};
}