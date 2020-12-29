#pragma once
#include "EaglePCH.h"
#include "Eagle/Rendering/Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Egl {
	Texture::Texture(const std::string& path, bool scaleUpBlur) : mPath(path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		EAGLE_ENG_ASSERT(data, "Failed to load an image!");
		mWidth = width;
		mHeight = height;

		GLenum glFormat = 0, dataFormat = 0;
		if (channels == 4) {
			glFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			glFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		EAGLE_ENG_ASSERT(glFormat != 0 || dataFormat != 0, "A texture isn't in format RGB or RGBA!");


		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, glFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (scaleUpBlur) glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		else			 glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		LOG_GL_STATUS("Texture init {0}", (unsigned int)mRendererID);
	}
	Texture::~Texture() {
		LOG_GL_STATUS("texture destroy {0}", (unsigned int)mRendererID);
		glDeleteTextures(1, &mRendererID);
	}
	void Texture::Bind(uint32_t slot) {
		LOG_GL_STATUS("texture bind {0}", (unsigned int)mRendererID);
		glBindTextureUnit(slot, mRendererID);
	}
}