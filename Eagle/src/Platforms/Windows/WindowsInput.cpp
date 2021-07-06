#include <EaglePCH.h>
#include <GLFW/glfw3.h>
#include "Eagle/Core/Input.h"
#include "Eagle/Core/Application.h"

namespace Egl {
	int Input::mMouseScrollX, Input::mMouseScrollY;

	bool Input::IsKeyPressed(int EGL_KEY) {
		auto window = Application::Get().GetWindow().NativeWindow();

		int key = glfwGetKey((GLFWwindow*)window, EGL_KEY);
		return key == GLFW_PRESS || key == GLFW_REPEAT;
	}
	bool Input::IsMousePressed(int EGL_MOUSE_BUTTON) {
		auto window = Application::Get().GetWindow().NativeWindow();

		int key = glfwGetMouseButton((GLFWwindow*)window, EGL_MOUSE_BUTTON);
		return key == GLFW_PRESS;
	}
	glm::vec2 Input::MousePos() {
		auto window = Application::Get().GetWindow().NativeWindow();
		double xPos, yPos;
		glfwGetCursorPos((GLFWwindow*)window, &xPos, &yPos);
		return Application::Get().WindowPixelToScenePixelSpace({ xPos, yPos });
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