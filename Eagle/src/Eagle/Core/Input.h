#pragma once
#include "Events/Event.h"

namespace Egl {
	class Input {
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMousePressed(int mouseButton);
		static float MousePosX() { return MousePos().x; }
		static float MousePosY() { return MousePos().y; }
		/// <summary>Has the mouse scroll moved vertically and in what direction. Doesn't need to be multiplied with deltaTime to normalize</summary>
		static int MouseScrolledY() { return mMouseScrollY; }
		/// <summary>Has the mouse scroll moved horizontally and in what direction. Doesn't need to be multiplied with deltaTime to normalize</summary>
		static int MouseScrolledX() { return mMouseScrollX; }
		static glm::vec2 MousePos();
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