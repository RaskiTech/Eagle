#include "EaglePCH.h"
#include "RenderAPI.h"

#include "Platforms/OpenGL/OpenGLRenderAPI.h"

namespace Egl {

	RenderAPI::API RenderAPI::sAPI = RenderAPI::API::OpenGL;

	Scope<RenderAPI> RenderAPI::Create()
	{
		switch (sAPI)
		{
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateScope<OpenGLRenderAPI>();
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}