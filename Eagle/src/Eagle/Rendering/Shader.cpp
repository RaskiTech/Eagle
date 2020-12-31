#include "EaglePCH.h"
#include "Eagle/Rendering/Shader.h"

#include "Eagle/Rendering/Renderer.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace Egl {
	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}