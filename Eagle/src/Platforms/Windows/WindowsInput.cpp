#include <EaglePCH.h>
#include <GLFW/glfw3.h>
#include "Eagle/Core/Input.h"
#include "Eagle/Core/Application.h"

namespace Egl {
	int Input::mMouseScrollX, Input::mMouseScrollY;

	bool Input::IsKeyPressed(int keycode) {
		auto window = Application::Get().GetWindow().NativeWindow();

		int key = glfwGetKey((GLFWwindow*)window, keycode);
		return key == GLFW_PRESS || key == GLFW_REPEAT;
	}
	bool Input::IsMousePressed(int mouseButton) {
		auto window = Application::Get().GetWindow().NativeWindow();

		int key = glfwGetMouseButton((GLFWwindow*)window, mouseButton);
		return key == GLFW_PRESS;
	}
	std::pair<float, float> Input::MousePos() {
		auto window = Application::Get().GetWindow().NativeWindow();
		double xPos, yPos;
		glfwGetCursorPos((GLFWwindow*)window, &xPos, &yPos);
		return std::pair<float, float>((float)xPos, (float)yPos);
	}
	void Input::OnEvent(Event& event) {
		if (event.GetEventType() == MouseScrolledEvent::GetStaticType()) {
			MouseScrolledEvent& scrollEvent = *(MouseScrolledEvent*)&event;
			mMouseScrollX = scrollEvent.GetScrollXOffset();
			mMouseScrollY = scrollEvent.GetScrollYOffset();
		}
	}
	void Input::ResetInputState() {
		mMouseScrollX = 0, mMouseScrollY = 0;
	}
}