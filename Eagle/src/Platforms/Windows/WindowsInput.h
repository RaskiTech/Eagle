#pragma once
#include "Eagle/Input.h"

namespace Egl {
	class WindowsInput : public Input {
	protected:
		virtual bool IsKeyPressedImp(int keycode) override;
		virtual bool IsMousePressedImp(int mouseButton) override;
		virtual std::pair<float, float> GetMousePosImp() override;
	};
}
