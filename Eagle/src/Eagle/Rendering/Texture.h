#pragma once
#include <string>
#include "Eagle/Core/Core.h"

namespace Egl {
	class Texture {
	public:
		Texture(const std::string& path, bool scaleUpBlur = true);
		~Texture();

		uint32_t GetWidth() { return mWidth; }
		uint32_t GetHeight() { return mHeight; }
		void Bind(uint32_t slot = 0);
		static inline Ref<Texture> Create(const std::string& path, bool scaleUpBlur = true) { return std::make_shared<Texture>(path, scaleUpBlur); }

	private:
		uint32_t mWidth, mHeight;
		std::string mPath;


#ifdef EAGLE_RENDERER_OPENGL
	private:
		uint32_t mRendererID;
#endif
	};
}