#include "EaglePCH.h"
#include "Eagle/Rendering/Texture.h"
#include "Eagle/Rendering/Renderer.h"

#include "Platforms/OpenGL/OpenGLTexture.h"

namespace Egl {

	Ref<Texture> Texture::Create(uint32_t width, uint32_t height, bool scaleUpBlur) {
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLTexture>(width, height, scaleUpBlur);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture> Texture::Create(const std::string& path, bool scaleUpBlur) {
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLTexture>(path, scaleUpBlur);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}