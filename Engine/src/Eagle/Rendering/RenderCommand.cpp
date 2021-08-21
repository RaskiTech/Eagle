#include "EaglePCH.h"
#include "RenderCommand.h"
#include "Platforms/OpenGL/OpenGLRenderAPI.h"

namespace Egl {
	RenderAPI* RenderCommand::sRenderer = new OpenGLRenderAPI();
}