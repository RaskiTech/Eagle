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

	class TextRenderer {
	public:
		TextRenderer() = default;

		void Init();
		void TextRenderer::RenderChars(const std::string& text);
	private:
		std::unordered_map<char, Character> characters;
	};
}
