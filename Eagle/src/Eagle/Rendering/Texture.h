#pragma once
#include <EaglePCH.h>
#include <string>
#include "Eagle/Core/Core.h"
#include "glm/glm.hpp"

namespace Egl {
	enum class TextureFormat {
		RGBA,     // 4 channels
		RGB,      // 3 channels
		Grayscale // 1 channel
	};

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		static Ref<Texture> Create(uint32_t width, uint32_t height, bool scaleUpBlur = false, bool tile = false, TextureFormat format = TextureFormat::RGBA);
		static Ref<Texture> Create(const std::string& path, bool scaleUpBlur = true, bool tile = false);

		virtual bool operator==(const Texture& other) const = 0;
	};

	///////////////////////////// SubTexture /////////////////////////////

	class SubTexture {
	public:
		static Ref<SubTexture> CreateFromIndexes(const Ref<Texture>& texture, const glm::vec2& subTextureIndexes, const glm::vec2& subTextureSize, const glm::vec2& cellsInSprite = { 1, 1 });
		SubTexture(const Ref<Texture>& texture, const glm::vec2& minCoords, const glm::vec2& maxCoods);

		const glm::vec2* GetTextureCoords() const { return mSubTextureCoords; }
		const Ref<Texture> GetTexture() const { return mTexture; }
	private:
		Ref<Texture> mTexture;
		glm::vec2 mSubTextureCoords[4];
	};
}