#include <EaglePCH.h>
#include "WindowsWindow.h"
#include "Eagle/Log.h"
#include "Eagle/Events/ApplicationEvent.h"
#include "Eagle/Events/KeyEvent.h"
#include "Eagle/Events/MouseEvent.h"
#include <glad/glad.h>

namespace Egl {
	static bool isGLFWInit = false;
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() { Shutdown(); }

	static void GLFWErrorCallback(int error, const char* description) {
		LOG_ENG_ERROR("GLFW ERROR! Errorcode: {0}", 0);
	}

	void WindowsWindow::Init(const WindowProps& props) {
		mData.Title = props.Title;
		mData.width = props.width;
		mData.height = props.height;

		//LOG_ENG_INFO("THIS works right");// props.Title, props.width, props.height);
		LOG_ENG_INFO("Creating window: {0} ({1}, {2})", props.Title, props.width, props.height);

		if (!isGLFWInit) {
			int success = glfwInit();
			EAGLE_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			isGLFWInit = true;
		}

		mWindow = glfwCreateWindow((int)props.width, (int)props.height, mData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(mWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EAGLE_ASSERT(status, "Failed to initialize glad");
		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);

		// Set events
		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scanmode, int action, int mods) {
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int character) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(character);
			data.EventCallback(event);
		});
		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS: {
					MousePressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double scrollX, double scrollY) {
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((int)scrollX, (int)scrollY);
			data.EventCallback(event);
		});
		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double posX, double posY) {
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((int)posX, (int)posY);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(mWindow);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}

	bool WindowsWindow::IsVSync() const {
		return mData.VSync;
	}
	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		mData.VSync = enabled;
	}
}