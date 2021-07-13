#include <EaglePCH.h>
#include "Eagle/Rendering/Renderer.h"
#include <freetype/freetype.h>
#include "TextRenderer.h"

// API: Construct a font object and use that to write text

// Make renderer support multiple shaders. This way you can allow client to create their own, and can also make a new shadew for text
// since it never has multiple colors. - This should be in text shader; Get the wanted color, and set alfa from the texture:
//   color = vec4(textColor, texture(sampler2D, TexCoords).r);

// Make text rendering use an atlas

namespace Egl {
	void TextRenderer::Init() {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			EAGLE_ENG_ASSERT(false, "Couldn't initialize FreeType");
			return;
		}
		FT_Face font;
		const char* path = "D:/Eagle/Eagle/Assets/Fonts/Roboto/Roboto-Regular.ttf";
		if (FT_New_Face(ft, path, 0, &font)) {
			LOG_ENG_ERROR("Couldn't load font from path {0}", path);
			return;
		}

		int fontSize = 48;
		FT_Set_Pixel_Sizes(font, 0, fontSize);

		// Load the first 128 characters

		for (unsigned char c = 0; c < 128; c++) {
			if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
				LOG_ENG_ERROR("Failed to load character {0}" + c);
				continue;
			}

			uint32_t width = font->glyph->bitmap.width;
			uint32_t height = font->glyph->bitmap.rows;
			Ref<Texture> tex = Texture::Create(width, height, true, false, TextureFormat::RGBA);

			// Convert 
			void* ptr = alloca(width * height * 4);
			uint8_t* currPtr = (uint8_t*)ptr;
			for (int i = 0; i < width * height; i++) {
				*currPtr = 255;
				currPtr++;
				*currPtr = 255;
				currPtr++;
				*currPtr = 255;
				currPtr++;
				*currPtr = font->glyph->bitmap.buffer[i];
				currPtr++;
			}

			tex->SetData(ptr, width*height*4);
			Character ch(tex, 
				glm::ivec2{ width, height }, 
				glm::ivec2{ font->glyph->bitmap_left, font->glyph->bitmap_top }, 
				font->glyph->advance.x);
			characters[c] = ch;
		}

		FT_Done_Face(font);
		FT_Done_FreeType(ft);
	}

	void TextRenderer::RenderChars(const std::string& text) {
		float scale = 0.03f;

		float x = 0;
		for (auto c = text.begin(); c != text.end(); c++) {
			Character charData = characters[*c];

			float xPos = x + charData.bearing.x * scale;
			float yPos = 0 - (charData.size.y - charData.bearing.y) * scale;

			x += (charData.advance >> 6) * scale;

			Renderer::DrawTextureQuad(0, glm::vec2{ xPos, yPos }, glm::vec2{ charData.size.x * scale, charData.size.y * scale }, charData.texture, 1.0f, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
		}

	}
}