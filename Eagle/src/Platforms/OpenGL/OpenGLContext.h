#pragma once
#include <GLFW/glfw3.h>
#include "Eagle/Rendering/RenderingContext.h"

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