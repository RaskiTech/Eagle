#pragma once
#include "Eagle/Rendering/RenderingContext.h"
#include "GLFW/glfw3.h"

struct GLFWwindow;

namespace Egl {
	class OpenGLContext : public RenderingContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* mWindowHandle;
	};
}