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
			TextureRef tex = Assets::CreateTexture(width, height, true, false);
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

			Assets::GetTexture(tex)->SendData(ptr, width*height*4);
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
		if (originalText.size() == 0)
			return;

		// Relative font size. Multiply by a small number to get the font size to more reasonable values
		float relativeFontSize = data.fontSize * cameraSize * 0.0001f;

		// Is text textWrap width up to date
		float thisMaxWidth = containerSize.x / relativeFontSize;
		if (lastMaxWidth != thisMaxWidth) {
			lastMaxWidth = thisMaxWidth;
			processedTest = AddLineBreaks(originalText, thisMaxWidth);
		}

		int lineNum = 0;
		glm::vec2 charPos;
		PlaceToNewLine(charPos, containerMiddle, containerSize, data.alignHorizontal, data.alignVertical, lineNum, relativeFontSize);
		uint32_t remainingChars = (uint32_t)data.charsVisible;
		for (const auto& c : processedTest) {
			if (c == '\n') {
				lineNum++;
				PlaceToNewLine(charPos, containerMiddle, containerSize, data.alignHorizontal, data.alignVertical, lineNum, relativeFontSize);
				continue;
			}

			if (--remainingChars < 0)
				break;
			
			EAGLE_ENG_ASSERT(0 <= c && c <= 128, "Character is out of range.");

			const Character& charData = characters[c];

			float xPos = charPos.x + charData.bearing.x * relativeFontSize;
			float yPos = charPos.y - (charData.size.y - charData.bearing.y) * relativeFontSize;
			glm::vec2 scale = { charData.size.x * relativeFontSize, charData.size.y * relativeFontSize };

			charPos.x += charData.advance * relativeFontSize;

			Renderer::DrawTextureQuad(sorting, glm::vec2{ xPos+scale.x/2, yPos+scale.y/2 }, scale, charData.texture, 1.0f, data.color);
		}
	}

#define CheckLineWrap() \
if (spaceLeft < 0) {    \
	wrapped << '\n';    \
	linePixelWidths.push_back(maxPixelWidth - spaceLeft - wordSize); \
	spaceLeft = maxPixelWidth - wordSize; \
}

	std::string TextRenderer::AddLineBreaks(const std::string& original, float maxPixelWidth) {
		EAGLE_PROFILE_FUNCTION();
		std::istringstream words(original);
		std::ostringstream wrapped;
		std::stringstream wordHolder;
		linePixelWidths.resize(0);
		
		char ch;
		float wordSize = 0;
		float spaceLeft = maxPixelWidth;
		while (words >> std::noskipws >> ch) {
			switch (ch) {
				case ' ':
					spaceLeft -= wordSize;
					CheckLineWrap()
					wordSize = 0;

					wrapped << wordHolder.str();
					wordHolder.str(std::string());
					wrapped << ch;
					spaceLeft -= characters[' '].advance;
					break;
				case '\n':
					spaceLeft -= wordSize;
					CheckLineWrap()
					wordSize = 0;

					wrapped << wordHolder.str();
					wordHolder.str(std::string());
					wrapped << ch;
					linePixelWidths.push_back(maxPixelWidth - spaceLeft);
					spaceLeft = maxPixelWidth;
					break;
				default:
					wordSize += characters[ch].advance;
					wordHolder << ch;
					break;
			}
		}
		// Put the final word in there
		spaceLeft -= wordSize;
		CheckLineWrap()
		wrapped << wordHolder.str();
		linePixelWidths.push_back(maxPixelWidth - spaceLeft);

		return wrapped.str();
	}

	inline void TextRenderer::PlaceToNewLine(glm::vec2& pos, const glm::vec2& containerMiddle, 
		const glm::vec2& containerSize, TextAlignHorizontal hor, TextAlignVertical ver, int lineIndex, float relativeFontSize) 
	{
		switch (hor) {
			case TextAlignHorizontal::Left:
				pos.x = containerMiddle.x - containerSize.x / 2; 
				break;
			case TextAlignHorizontal::Middle:
				pos.x = containerMiddle.x - linePixelWidths[lineIndex] / 2 * relativeFontSize;
				break;
			case TextAlignHorizontal::Right:
				pos.x = containerMiddle.x + containerSize.x / 2 - linePixelWidths[lineIndex] * relativeFontSize;
				break;
		}
		switch (ver) {
			case TextAlignVertical::Top:
				pos.y = containerMiddle.y + containerSize.y / 2 - (fontHeight + fontDescend) * lineIndex * relativeFontSize - fontHeight * relativeFontSize;
				break;
			case TextAlignVertical::Middle:
				pos.y = containerMiddle.y + (fontHeight + fontDescend) * ((float)(linePixelWidths.size()-1) / 2 - lineIndex) * relativeFontSize;
				break;
			case TextAlignVertical::Bottom:
				pos.y = containerMiddle.y - containerSize.y / 2 + (fontHeight + fontDescend) * (linePixelWidths.size()-1-lineIndex) * relativeFontSize + fontDescend * relativeFontSize;
				break;
		}
	}
}