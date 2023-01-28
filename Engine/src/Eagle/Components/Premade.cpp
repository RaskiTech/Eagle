#include <EaglePCH.h>
#include "Premade.h"
#include "Eagle/ECS/Components.h"

namespace Egl
{
	namespace Premade
	{

		ButtonBuilder::ButtonBuilder(Entity parent, std::string_view buttonName, Button&& buttonComponent, UIEntityParams& params)
			: _button(parent.GetScene()->AddUIEntity(buttonName, parent, params))
		{
			_button.AddComponent<Button>(std::forward<Button&&>(buttonComponent));
			_button.AddComponent<TextComponent>();
			_button.AddComponent<SpriteRendererComponent>(buttonComponent.baseColor);

			_buttonComp = &_button.GetComponent<Button>();
			_textComp = &_button.GetComponent<TextComponent>();
			_spriteComp = &_button.GetComponent<SpriteRendererComponent>();

			auto buttonCallback = [](Entity entity, Event& event) {
				auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
				auto& buttonComp = entity.GetComponent<Button>();

				if (auto mouseEvent = event.FilterAs<MousePressedEvent>())
				{
					spriteComp.color = buttonComp.pressedColor;
					buttonComp.currentlyPressed = true;
					return true;
				}
				else if (auto mouseEvent = event.FilterAs<MouseReleasedEvent>())
				{
					// Click

					if (!buttonComp.currentlyPressed)
						return false;

					entity.GetComponent<Button>().callback();
					spriteComp.color = buttonComp.hoverColor;
					buttonComp.currentlyPressed = false;
					return true;
				}
				else if (auto enterEvent = event.FilterAs<MouseHoverEnterEvent>())
				{
					spriteComp.color = buttonComp.hoverColor;
					return false;
				}
				else if (auto exitEvent = event.FilterAs<MouseHoverExitEvent>())
				{
					spriteComp.color = buttonComp.baseColor;
					buttonComp.currentlyPressed = false;
					return false;
				}

				return false;
			};

			parent.GetScene()->SubscribeToEvents(_button, buttonCallback);
			parent.GetScene()->SubscribeToEnterExitEvents(_button, buttonCallback);
		}

		SliderBuilder::SliderBuilder(Entity parent, std::string_view sliderName, Slider&& sliderComponent, UIEntityParams& params)
			: _slider(parent.GetScene()->AddUIEntity(sliderName, parent, params))
		{
			Entity sliderHandle = AddButton(_slider, "Handle", Button(sliderComponent.baseColor, sliderComponent.hoverColor, sliderComponent.pressedColor, []() {}));

			_sliderComp.AddComponent<SpriteRendererComponent>(buttonComponent.baseColor);


		}

	}
}
