#pragma once
#include <EaglePCH.h>
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Egl {
	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, bool scaleUpBlur) : mWidth(width), mHeight(height) {
		mInternalFormat = GL_RGBA8;
		mDataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (scaleUpBlur) glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		else			 glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	OpenGLTexture::OpenGLTexture(const std::string& path, bool scaleUpBlur) : mPath(path), mDataFormat(0), mInternalFormat(0) {
		EAGLE_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			EAGLE_PROFILE_SCOPE("stri_load - Texture::Texture(const std::string& path, bool scaleUpBlur)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		EAGLE_ENG_ASSERT(data, "Failed to load an image!");
		mWidth = width;
		mHeight = height;

		if (channels == 4) {
			mInternalFormat = GL_RGBA8;
			mDataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			mInternalFormat = GL_RGB8;
			mDataFormat = GL_RGB;
		}
		EAGLE_ENG_ASSERT(mInternalFormat != 0 || mDataFormat != 0, "A texture isn't in format RGB or RGBA!");


		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (scaleUpBlur) glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		else			 glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	OpenGLTexture::~OpenGLTexture() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteTextures(1, &mRendererID);
	}
	void OpenGLTexture::SetData(void* data, uint32_t size) {
		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture::Bind(uint32_t slot) const {
		EAGLE_PROFILE_FUNCTION();
		glBindTextureUnit(slot, mRendererID);
	}
}