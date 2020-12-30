#pragma once
#include <string>
#include "Eagle/Core/Core.h"

#ifdef EAGLE_RENDERER_OPENGL
	#include <glad/glad.h>
#endif

namespace Egl {
	class Texture {
	public:
		Texture(uint32_t width, uint32_t height, bool scaleUpBlur = true);
		Texture(const std::string& path, bool scaleUpBlur = true);
		~Texture();

		uint32_t GetWidth() { return mWidth; }
		uint32_t GetHeight() { return mHeight; }

		virtual void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0);
		static inline Ref<Texture> Create(const std::string& path, bool scaleUpBlur = true) { return CreateRef<Texture>(path, scaleUpBlur); }
		static inline Ref<Texture> Create(uint32_t width, uint32_t height, bool scaleUpBlur = true) { return CreateRef<Texture>(width, height, scaleUpBlur); }
	private:
		uint32_t mWidth;
		uint32_t mHeight;
		std::string mPath;


#ifdef EAGLE_RENDERER_OPENGL
	private:
		uint32_t mRendererID;
		GLenum mInternalFormat;
		GLenum mDataFormat;
#endif
	};
}