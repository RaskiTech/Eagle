#pragma once
#include "Events/Event.h"

namespace Egl {
	class Input {
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMousePressed(int mouseButton);
		static float MousePosX() { auto [x, y] = MousePos(); return x; }
		static float MousePosY() { auto [x, y] = MousePos(); return y; }
		static int MouseScrolledY() { return mMouseScrollY; }
		static int MouseScrolledX() { return mMouseScrollX; }
		static std::pair<float, float> MousePos();
	private:
		friend class GameLayer;
		static void OnEvent(Event& event);
	protected:
		friend class WindowsWindow;
		static void ResetInputState();

	private:
		static int mMouseScrollX, mMouseScrollY;
	};
}