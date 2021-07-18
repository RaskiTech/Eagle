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
	void TextRenderer::LoadFont(const std::string& fontPath) {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			EAGLE_ENG_ASSERT(false, "Couldn't initialize FreeType");
			return;
		}

		FT_Face font;
		if (FT_New_Face(ft, fontPath.c_str(), 0, &font)) {
			LOG_ENG_ERROR("Couldn't load font from path {0}", fontPath);
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
			Ref<Texture> tex = Texture::Create(width, height, true, false);
			fontHeight = (float)(font->height >> 6);
			fontDescend = (float)(-font->descender >> 6);

			// Convert from R to RGBA
			void* ptr = malloc((size_t)width * height * 4);
			uint8_t* currPtr = (uint8_t*)ptr;
			for (uint32_t i = 0; i < width * height; i++) {
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
			free(ptr);

			Character ch(tex, 
				glm::ivec2{ width, height }, 
				glm::ivec2{ font->glyph->bitmap_left, font->glyph->bitmap_top }, 
				font->glyph->advance.x >> 6);
			characters[c] = ch;
		}

		FT_Done_Face(font);
		FT_Done_FreeType(ft);
	}

	void TextRenderer::ChangeRenderedText(const std::string& unprocessedText) {
		originalText = unprocessedText;
		lastMaxWidth = 0; // Dirty flag in disguise
	}

	// TODO:
	// leftSideWallPos
	// align V & H
	void TextRenderer::RenderText(const uint16_t& sorting, const TextProperties& data, const glm::vec2& containerMiddle, const glm::vec2& containerSize, float cameraSize) {
		EAGLE_PROFILE_FUNCTION();

		// Relative font size. Multiply by a small number to get the font size to more reasonable values
		float relativeFontSize = data.fontSize * cameraSize * 0.0001f;

		// Is text textWrap width up to date
		float thisMaxWidth = containerSize.x / relativeFontSize * 0.75f;
		if (lastMaxWidth != thisMaxWidth) {
			lastMaxWidth = thisMaxWidth;
			processedTest = AddLineBreaks(originalText, thisMaxWidth);
		}

		float charPosX;
		float charPosY;
		//switch (data.alignVertical) {
		//	case TextAlignVertical::Top: charPosY = containerMiddle.y + containerSize.y / 2 - fontHeight * relativeFontSize;
		//}
		//switch (data.alignHorizontal) {
		//	case TextAlignHorizontal::Left: charPosX = containerMiddle.x - containerSize.x / 2;
		//}
		charPosX = containerMiddle.x - containerSize.x / 2;
		charPosY = containerMiddle.y + containerSize.y / 2 - fontHeight * relativeFontSize;

		float remainingChars = data.charsVisible;
		for (const auto& c : processedTest) {
			if (c == '\n') {
				charPosY -= (fontHeight + fontDescend) * relativeFontSize;
				charPosX = containerMiddle.x - containerSize.x / 2;
				continue;
			}

			if (--remainingChars < 0)
				break;

			const Character& charData = characters[c];

			float xPos = charPosX + charData.bearing.x * relativeFontSize;
			float yPos = charPosY - (charData.size.y - charData.bearing.y) * relativeFontSize;
			glm::vec2 scale = { charData.size.x * relativeFontSize, charData.size.y * relativeFontSize };

			charPosX += charData.advance * relativeFontSize;

			Renderer::DrawTextureQuad(sorting, glm::vec2{ xPos+scale.x/2, yPos+scale.y/2 }, scale, charData.texture, 1.0f, data.color);
		}
	}

	float TextRenderer::GetWordSize(const std::string& word) {
		float size = 0;
		for (auto& c : word) {
			size += characters[c].size.x;
		}
		return size;
	}
	std::string TextRenderer::AddLineBreaks(const std::string& original, uint32_t maxPixelWidth) {
		std::istringstream words(original);
		std::ostringstream wrapped;

		std::string word;

		linePixelWidths.resize(0);

		if (words >> word) {
			wrapped << word;
			size_t spaceLeft = maxPixelWidth - GetWordSize(word);
			while (words >> word) {
				float wordSize = GetWordSize(word);
				if (spaceLeft < wordSize) {
					wrapped << "\n" << word;
					linePixelWidths.push_back(maxPixelWidth - spaceLeft);
					spaceLeft = maxPixelWidth - wordSize;
				}
				else {
					wrapped << ' ' << word;
					spaceLeft -= wordSize;
				}
			}
		}
		return wrapped.str();
	}
}