#include <EaglePCH.h>
#include "Framebuffer.h"
#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLFramebuffer.h"

namespace Egl {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferDefenition& defenition)
	{
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(defenition);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
		return Ref<Framebuffer>();
	}

	bool Framebuffer::IsDepthFormat(const FramebufferTextureFormat& format) {
		return format == FramebufferTextureFormat::DEPTH24STENCIL8;
	}

}