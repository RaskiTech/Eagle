#pragma once
#include <EaglePCH.h>
#include "Eagle/ECS/Entity.h"
#include "Eagle/ECS/Components.h"

namespace Egl
{
	namespace UI
	{
		class ButtonBuilder
		{
		public:
			ButtonBuilder(Entity parent, std::string_view buttonName, Button&& buttonComponent, UIEntityParams& params);
			ButtonBuilder(ButtonBuilder& other) = delete;

			inline ButtonBuilder& SetFont(FontRef font) { _textComp->textRenderer.SetFont(font); return *this; }
			inline ButtonBuilder& SetText(std::string_view buttonText) { _textComp->SetText(buttonText); return *this; }
			inline ButtonBuilder& SetFontSize(float size) { _textComp->props.fontSize = size; return *this; }
			inline ButtonBuilder& SetTextColor(const glm::vec4& color) { _textComp->props.color = color; return *this; }
			inline ButtonBuilder& SetTexture(TextureRef texture) { _spriteComp->texture = Assets::LoadSubTexture(texture, { 0, 0 }, { 1, 1 }); return *this; }
			inline ButtonBuilder& SetTexture(SubTextureRef texture) { _spriteComp->texture = texture; return *this; }

			// This can be called when finished building the button, but the builder can also just be assigned to an entity
			inline Entity Finish() { return _button; }
			operator Entity() { return Finish(); }

		private:
			Entity _button;
			SpriteRendererComponent* _spriteComp;
			TextComponent* _textComp;
			Button* _buttonComp;
		};

		class SliderBuilder
		{
		public:
			SliderBuilder(Entity parent, std::string_view sliderName, Slider&& sliderComponent, UIEntityParams& params);
			SliderBuilder(SliderBuilder& other) = delete;

			inline SliderBuilder& SetBaseColor(const glm::vec4& color) { _baseSpriteComp->color = color; }

			// This can be called when finished building the button, but the builder can also just be assigned to an entity
			inline Entity Finish() { return _slider; }
			operator Entity() { return Finish(); }

		private:
			Entity _slider;
			SpriteRendererComponent* _baseSpriteComp;
			SpriteRendererComponent* _handleSpriteComp;
			Slider* _sliderComp;
			Button* _buttonComp;
		};


		// Returns a bunch of functions that be used to modify the button. Once all wanted functions have been called, store the button as an entity, not as a ButtonBuilder
		inline ButtonBuilder AddButton(Entity parent, std::string_view buttonName, Button&& buttonComponent, UIEntityParams& params = UIEntityParams()) { return ButtonBuilder(parent, buttonName, std::forward<Button&&>(buttonComponent), params); }
		// Returns a bunch of functions that be used to modify the slider. Once all wanted functions have been called, store the slider as an entity, not as a ButtonBuilder
		inline SliderBuilder AddSlider(Entity parent, std::string_view buttonName, Slider&& buttonComponent, UIEntityParams& params = UIEntityParams()) { return SliderBuilder(parent, buttonName, std::forward<Slider&&>(buttonComponent), params); }
	}
}
