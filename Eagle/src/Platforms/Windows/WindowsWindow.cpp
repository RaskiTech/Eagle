#include <EaglePCH.h>
#include "WindowsWindow.h"
#include "Eagle/Core/Events/ApplicationEvent.h"
#include "Eagle/Core/Events/KeyEvent.h"
#include "Eagle/Core/Events/MouseEvent.h"
#include "Platforms/OpenGL/OpenGLContext.h"

namespace Egl {
	static uint8_t sGLFWwindowCount = 0;
	Scope<Window> Window::Create(const WindowProps& props) {
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		EAGLE_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow() { Shutdown(); }

	static void GLFWErrorCallback(int error, const char* description) {
		LOG_ENG_ERROR("GLFW ERROR! {0}: {1}", error, description);
	}

	void WindowsWindow::Init(const WindowProps& props) {
		EAGLE_PROFILE_FUNCTION();

		mData.Title = props.Title;
		mData.width = props.width;
		mData.height = props.height;

		if (sGLFWwindowCount == 0) {
			{
				EAGLE_PROFILE_SCOPE("glfwInit");
				int success = glfwInit();
				EAGLE_ENG_ASSERT(success, "Could not intialize GLFW!");
			}

			glfwSetErrorCallback(GLFWErrorCallback);
		}
		sGLFWwindowCount++;

		{
			EAGLE_PROFILE_SCOPE("glfwCreateWindow");
			mWindow = glfwCreateWindow((int)props.width, (int)props.height, mData.Title.c_str(), nullptr, nullptr);
		}


		mContext = new OpenGLContext(mWindow);
		mContext->Init();

		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(false);

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
	
	static void WindowSizeCallback(GLFWwindow* window, int width, int height) {
		LOG("RESIZE ATTEMT");
		//WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);
		//data.width = width;
		//data.height = height;
		//auto data = *(WindowData*)glfwGetWindowUserPointer(window);
		//
		//WindowResizeEvent event(width, height);
		//data.EventCallback(event);
		//mContext->SwapBuffers();
	}

	void WindowsWindow::Shutdown() {
		EAGLE_PROFILE_FUNCTION();

		glfwDestroyWindow(mWindow);
		sGLFWwindowCount--;

		if (sGLFWwindowCount == 0)
			glfwTerminate();
	}

	void WindowsWindow::OnUpdate() {
		EAGLE_PROFILE_FUNCTION();

		glfwPollEvents();
		Render();
	}
	void WindowsWindow::Render() {
		mContext->SwapBuffers();
	}

	bool WindowsWindow::IsVSync() const {
		return mData.VSync;
	}
	void WindowsWindow::SetVSync(bool enabled) {
		EAGLE_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		mData.VSync = enabled;
	}
}