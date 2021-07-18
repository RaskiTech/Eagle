#pragma once
#include <glm/glm.hpp>
#include "Eagle/Rendering/Texture.h"

namespace Egl {
	struct Character {
		Character() = default;
		Character(Ref<Texture> texture, const glm::ivec2& size, const glm::ivec2& bearing, uint32_t advance)
			: texture(texture), size(size), bearing(bearing), advance(advance) {}
		Ref<Texture> texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		uint32_t advance;
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
		float fontSize = 0.01f;
		TextAlignHorizontal alignHorizontal = TextAlignHorizontal::Left;
		TextAlignVertical alignVertical = TextAlignVertical::Top;
		glm::vec4 color = { 1, 1, 1, 1 };
		uint32_t charsVisible = -1;
	};

	class TextRenderer {
	public:
		TextRenderer(const std::string& fontPath) { LoadFont(fontPath); linePixelWidths.reserve(20); };
		void LoadFont(const std::string& fontPath);

		void ChangeRenderedText(const std::string& unprocessedText);
		const std::string& GetOriginalText() const { return originalText; }

		void RenderText(const uint16_t& sorting, const TextProperties& data, const glm::vec2& containerMiddle, const glm::vec2& containerSize, float cameraSize);
	private:
		float GetWordSize(const std::string& word);
		std::string AddLineBreaks(const std::string& original, uint32_t maxPixelWidth);

		float lastMaxWidth = 0;
		std::string originalText = "";
		std::string processedTest = "";
		std::vector<uint16_t> linePixelWidths;

		float fontHeight = 0;
		float fontDescend = 0;
		std::unordered_map<char, Character> characters;
	};
}
