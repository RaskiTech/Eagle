#include <EaglePCH.h>
#include "FrameBuffer.h"
#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLFrameBuffer.h"

namespace Egl {

	Ref<FrameBuffer> Egl::FrameBuffer::Create(const FrameBufferDefenition& defenition)
	{
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLFrameBuffer>(defenition);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
		return Ref<FrameBuffer>();
	}

}