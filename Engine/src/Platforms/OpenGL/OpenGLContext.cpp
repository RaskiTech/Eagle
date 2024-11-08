#include <EaglePCH.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "OpenGLContext.h"
#include "Eagle/Core/Core.h"

namespace Egl {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: mWindowHandle(windowHandle)
	{
		EAGLE_ENG_ASSERT(windowHandle, "WindowHandle is not defined, it's null!")
	}
	void OpenGLContext::Init()
	{
		EAGLE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(mWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EAGLE_ASSERT(status, "Failed to initialize glad");

		LOG_ENG("OpenGL Renderer: ");
		LOG_ENG("	Vendor:", glGetString(GL_VENDOR));
		LOG_ENG("	Renderer:", glGetString(GL_RENDERER));
		LOG_ENG("	Version:", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		EAGLE_PROFILE_SCOPE("OpenGLContext::SwapBuffers() - Also waiting for VSync");
		glfwSwapBuffers(mWindowHandle);
	}
}