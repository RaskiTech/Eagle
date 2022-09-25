#pragma once
#include <EaglePCH.h>
#include <string>
#include "Eagle/Core/Core.h"
#include "Eagle/Core/AssetManager.h"
#include <glm/glm.hpp>

namespace Egl {
	//enum class TextureFormat {
	//	RGBA,     // 4 channels
	//	RGB,      // 3 channels
	//	Grayscale // 1 channel
	//};

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		
		virtual void SendData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		static Texture* CreateDangling(uint32_t width, uint32_t height, bool scaleUpBlur = false, bool tile = false);
		static Texture* CreateDangling(const std::string& path, bool scaleUpBlur = true, bool tile = false);

		virtual bool operator==(const Texture& other) const = 0;
	};

	///////////////////////////// SubTexture /////////////////////////////

	class SubTexture {
	public:
		static SubTexture* CreateDanglingFromIndexes(TextureRef texture, const glm::vec2& subTextureIndexes, const glm::vec2& subTextureSize, const glm::vec2& cellsInSprite = { 1, 1 });
		static SubTexture* CreateDangling(TextureRef texture, const glm::vec2& minCoord, const glm::vec2& maxCoords) { return new SubTexture(texture, minCoord, maxCoords); }

		const glm::vec2* GetTextureCoords() const { return mSubTextureCoords; }
		const TextureRef GetTexture() const { return mTexture; }
	private:
		SubTexture(TextureRef texture, const glm::vec2& minCoords, const glm::vec2& maxCoods);

		TextureRef mTexture;
		glm::vec2 mSubTextureCoords[4];
	};
}