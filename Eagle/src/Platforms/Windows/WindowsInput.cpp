#include "EaglePCH.h"
#include "WindowsInput.h"
#include <GLFW/glfw3.h>
#include "Eagle/Application.h"

namespace Egl {
	Input* Input::mInstance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImp(int keycode) {
		auto window = Application::Get().GetWindow().NativeWindow();

		int key = glfwGetKey((GLFWwindow*)window, keycode);
		return key == GLFW_PRESS || key == GLFW_REPEAT;
	}
	bool WindowsInput::IsMousePressedImp(int mouseButton) {
		auto window = Application::Get().GetWindow().NativeWindow();

		int key = glfwGetMouseButton((GLFWwindow*)window, mouseButton);
		return key == GLFW_PRESS;
	}
	std::pair<float, float> WindowsInput::GetMousePosImp() {
		auto window = Application::Get().GetWindow().NativeWindow();
		double xPos, yPos;
		glfwGetCursorPos((GLFWwindow*)window, &xPos, &yPos);
		return std::pair<float, float>((float)xPos, (float)yPos);
	}
}