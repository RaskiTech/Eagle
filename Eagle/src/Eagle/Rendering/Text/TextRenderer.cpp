#include <EaglePCH.h>
#include "Eagle/Rendering/Renderer.h"
#include "TextRenderer.h"

// API: Construct a font object and use that to write text

namespace Egl {
	void TextRenderer::Init() {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			EAGLE_ENG_ASSERT(false, "Couldn't initialize FreeType");
			return;
		}

		const char* path = "Eagle/Assets/Fonts/Roboto/Roboto-Regular.ttf";
		if (FT_New_Face(ft, path, 0, &font)) {
			LOG_ENG_ERROR("Couldn't load font from path {0}", path);
			return;
		}

		int fontSize = 48;
		FT_Set_Pixel_Sizes(font, 0, fontSize);

		return;
		// TODO: Make texture API accept grayScale images

		// Load the first 128 characters
		for (unsigned char c = 0; c < 128; c++) {
			if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
				LOG_ENG_ERROR("Failed to load character {0}" + c);
				continue;
			}

			uint32_t width = font->glyph->bitmap.width;
			uint32_t height = font->glyph->bitmap.rows;
			Ref<Texture> tex = Texture::Create(width, height, true);
			tex->SetData((void*)font->glyph->bitmap.buffer, width*height);
			Character ch(tex, 
				glm::ivec2{ width, height }, 
				glm::ivec2{ font->glyph->bitmap_left, font->glyph->bitmap_top }, 
				font->glyph->advance.x);
			characters[c] = ch;
		}

	}
}