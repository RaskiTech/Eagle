#include <EaglePCH.h>
#include "Premade.h"
#include "Eagle/ECS/Components.h"

namespace Egl
{
	namespace UI
	{

		ButtonBuilder::ButtonBuilder(Entity parent, std::string_view buttonName, Button&& buttonComponent, UIEntityParams& params)
			: button(parent.GetScene()->AddUIEntity(buttonName, params, parent))
		{
			button.AddComponent<Button>(std::forward<Button&&>(buttonComponent));
			button.AddComponent<TextComponent>();
			button.AddComponent<SpriteRendererComponent>(buttonComponent.baseColor);

			buttonComp = &button.GetComponent<Button>();
			textComp = &button.GetComponent<TextComponent>();
			spriteComp = &button.GetComponent<SpriteRendererComponent>();

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

			parent.GetScene()->SubscribeToEvents(button, buttonCallback);
			parent.GetScene()->SubscribeToEnterExitEvents(button, buttonCallback);
		}

	}
}
