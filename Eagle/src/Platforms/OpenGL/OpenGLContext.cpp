#include "EaglePCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Egl {
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(mWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EAGLE_ASSERT(status, "Failed to initialize glad");

		LOG_ENG("OpenGL Renderer: ");
		LOG_ENG("	Vendor: {0}", glGetString(GL_VENDOR));
		LOG_ENG("	Renderer: {0}", glGetString(GL_RENDERER));
		LOG_ENG("	Version: {0}", glGetString(GL_VERSION));
	}
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: mWindowHandle(windowHandle)
	{
		EAGLE_CORE_ASSERT(windowHandle, "WindowHandle is not defined, it's null!")
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindowHandle);
	}
}