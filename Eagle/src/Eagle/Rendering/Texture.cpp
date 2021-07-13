#include "EaglePCH.h"
#include "Eagle/Rendering/Texture.h"
#include "Eagle/Rendering/Renderer.h"

#include "Platforms/OpenGL/OpenGLTexture.h"

namespace Egl {

	Ref<Texture> Texture::Create(uint32_t width, uint32_t height, bool scaleUpBlur, bool tile, TextureFormat format) {
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLTexture>(width, height, scaleUpBlur, tile, format);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture> Texture::Create(const std::string& path, bool scaleUpBlur, bool tile) {
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLTexture>(path, scaleUpBlur, tile);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	///////////////////////////// SubTexture /////////////////////////////

	/// <summary> If the sub-textures in the texture form a grid, this can be used. </summary>
	Ref<SubTexture> SubTexture::CreateFromIndexes(const Ref<Texture>& texture, const glm::vec2& subTextureIndexes, const glm::vec2& gridCellSize, const glm::vec2& cellsInSprite) {
		glm::vec2 min = { subTextureIndexes.x * gridCellSize.x / texture->GetWidth(), subTextureIndexes.y * gridCellSize.y / texture->GetHeight() };
		glm::vec2 max = { (subTextureIndexes.x + cellsInSprite.x) * gridCellSize.x / texture->GetWidth(), (subTextureIndexes.y + cellsInSprite.y) * gridCellSize.y / texture->GetHeight() };
		return CreateRef<SubTexture>(texture, min, max);
	}

	SubTexture::SubTexture(const Ref<Texture>& texture, const glm::vec2& minCoords, const glm::vec2& maxCoords) : mTexture(texture)  {
		mSubTextureCoords[0] = minCoords;
		mSubTextureCoords[1] = { maxCoords.x, minCoords.y };
		mSubTextureCoords[2] = maxCoords;
		mSubTextureCoords[3] = { minCoords.x, maxCoords.y };
	}

}