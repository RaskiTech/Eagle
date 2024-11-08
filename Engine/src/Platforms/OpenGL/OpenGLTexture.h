#pragma once

#include "Eagle/Rendering/Texture.h"
#include <glad/glad.h>

namespace Egl {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(uint32_t width, uint32_t height, bool scaleUpBlur, bool tile);
		OpenGLTexture(const std::string& path, bool scaleUpBlur, bool tile);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }
		virtual uint32_t GetRendererID() const override { return mRendererID; }

		virtual void SendData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override { return mRendererID == ((OpenGLTexture&)other).mRendererID; }
	private:
		std::string mPath;
		uint32_t mWidth, mHeight;
		uint32_t mRendererID;
		GLenum mInternalFormat, mDataFormat;
	};

}