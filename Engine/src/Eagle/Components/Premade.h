#pragma once
#include <EaglePCH.h>
#include "Eagle/ECS/Entity.h"
#include "Eagle/ECS/Components.h"

namespace Egl
{
	namespace UI
	{
		struct Button
		{
			glm::vec4 baseColor{ 0 };
			glm::vec4 hoverColor{ 0 };
			glm::vec4 pressedColor{ 0 };

			std::function<void()> callback = nullptr;

			bool currentlyPressed = false;
		};

		class ButtonBuilder
		{
		public:
			ButtonBuilder(Entity parent, std::string_view buttonName, Button&& buttonComponent, UIEntityParams& params);
			ButtonBuilder(ButtonBuilder& other) = delete;
			operator Entity() { return Finish(); }

			inline ButtonBuilder& SetFont(FontRef font) { textComp->textRenderer.SetFont(font); return *this; }
			inline ButtonBuilder& SetText(std::string_view buttonText) { textComp->SetText(buttonText); return *this; }
			inline ButtonBuilder& SetFontSize(float size) { textComp->props.fontSize = size; return *this; }
			inline ButtonBuilder& SetTextColor(const glm::vec4& color) { textComp->props.color = color; return *this; }

			inline Entity Finish() { return button; }

		private:
			Entity button;
			SpriteRendererComponent* spriteComp;
			TextComponent* textComp;
			Button* buttonComp;
		};

		// Returns a bunch of functions that be used to modify the button. Once all wanted functions have been called, store the button as an entity, not as a ButtonBuilder
		inline ButtonBuilder AddButton(Entity parent, std::string_view buttonName, Button&& buttonComponent, UIEntityParams& params = UIEntityParams()) { return ButtonBuilder(parent, buttonName, std::forward<Button&&>(buttonComponent), params); }

	}
}
