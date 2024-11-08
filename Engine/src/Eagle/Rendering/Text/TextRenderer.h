#pragma once
#include <Dependencies/GLM.h>
#include "Eagle/Rendering/Texture.h"

namespace Egl {
	struct Character {
		Character() = default;
		Character(TextureRef texture, const glm::ivec2& size, const glm::ivec2& bearing, uint32_t advance)
			: texture(texture), size(size), bearing(bearing), advance(advance) {}
		TextureRef texture = -1;
		glm::ivec2 size = { 0, 0 };
		glm::ivec2 bearing = { 0, 0 };
		uint32_t advance = 0;
	};

	enum class TextAlignVertical {
		Top,
		Middle,
		Bottom
	};
	enum class TextAlignHorizontal {
		Left,
		Middle,
		Right
	};
	struct TextProperties {
		TextProperties() = default;

		float fontSize = 5;
		TextAlignHorizontal alignHorizontal = TextAlignHorizontal::Middle;
		TextAlignVertical alignVertical = TextAlignVertical::Middle;
		glm::vec4 color = { 1, 1, 1, 1 };
		int charsVisible = -1;
	};

	struct FontData
	{
		FontData(const std::string& fontPath) { LoadFont(fontPath); }

		void LoadFont(const std::string& fontPath);


		float fontHeight = 0;
		float fontDescend = 0;
		std::unordered_map<char, Character> characters;
	};

	class TextRenderer {
	public:
		TextRenderer(FontRef font) : _font(font) { _linePixelWidths.reserve(20); };
		TextRenderer() = default;
		~TextRenderer() = default;

		void ChangeRenderedText(std::string_view unprocessedText);
		const std::string& GetOriginalText() const { return _originalText; }

		void SetFont(FontRef font) { _font = font; }

		void RenderText(const uint16_t& sorting, const TextProperties& data, const glm::vec2& containerMiddle, const glm::vec2& containerSize, float cameraSize);
	private:
		std::string AddLineBreaks(const std::string& original, float maxPixelWidth);

		inline void PlaceToNewLine(glm::vec2& pos, const glm::vec2& containerMiddle, const glm::vec2& containerSize, 
			TextAlignHorizontal hor, TextAlignVertical ver, int lineIndex, float relativeFontSize);

		float _lastMaxWidth = 0;
		std::string _originalText = "";
		std::string _processedTest = "";
		std::vector<float> _linePixelWidths;
		FontRef _font;
	};

}
