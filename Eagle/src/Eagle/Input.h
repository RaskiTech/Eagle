#pragma once
#include "Eagle/Core.h"
namespace Egl {
	class Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return mInstance->IsKeyPressedImp(keycode); }
		inline static bool IsMousePressed(int mouseButton) { return mInstance->IsMousePressedImp(mouseButton); }
		inline static float MousePosX() { auto [x, y] = MousePos(); return x; }
		inline static float MousePosY() { auto [x, y] = MousePos(); return y; }
		inline static std::pair<float, float> MousePos() { return mInstance->GetMousePosImp(); }
	protected:
		virtual bool IsKeyPressedImp(int keycode) = 0;
		virtual bool IsMousePressedImp(int mouseButton) = 0;
		virtual std::pair<float, float> GetMousePosImp() = 0;
	private:
		static Input* mInstance;
	};
}