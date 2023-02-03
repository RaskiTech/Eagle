#include <EaglePCH.h>
#include "Eagle/Core/Application.h"
#include "Premade.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/Core/Input.h"

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

				if (auto pressed = event.FilterAs<MousePressedEvent>())
				{
					spriteComp.color = buttonComp.pressedColor;
					buttonComp.heldDown = true;
					return true;
				}
				else if (auto released = event.FilterAs<MouseReleasedEvent>())
				{
					if (buttonComp.heldDown && buttonComp.hovering)
					{
						entity.GetComponent<Button>().callback();
					}

					if (buttonComp.hovering)
						spriteComp.color = buttonComp.hoverColor;
					else
						spriteComp.color = buttonComp.baseColor;
					buttonComp.heldDown = false;
					return true;
				}
				else if (auto enterEvent = event.FilterAs<MouseHoverEnterEvent>())
				{
					if (!buttonComp.heldDown)
						spriteComp.color = buttonComp.hoverColor;
					buttonComp.hovering = true;
					return true;
				}
				else if (auto exitEvent = event.FilterAs<MouseHoverExitEvent>())
				{
					if (!buttonComp.heldDown)
						spriteComp.color = buttonComp.baseColor;
					buttonComp.hovering = false;
					return true;
				}
				


				return false;
			};

			//parent.GetScene()->GetUIManager().AddUIElement(_button, buttonCallback);

			_buttonComp->ActivateUIElement(_button, buttonCallback);

			//parent.GetScene()->SubscribeToEvents(_button, buttonCallback);
			//parent.GetScene()->SubscribeToEnterExitEvents(_button, buttonCallback);
		}

		SliderBuilder::SliderBuilder(Entity parent, std::string_view sliderName, Slider&& sliderComponent, UIEntityParams& params)
			: _slider(parent.GetScene()->AddUIEntity(sliderName, parent, params))
		{
			auto handleCallback = [](Entity entity, Event& event) {
				auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
				auto& buttonComp = entity.GetComponent<Button>();
				Entity baseEntity = entity.GetParent();
				auto& sliderComp = baseEntity.GetComponent<Slider>();

				if (auto pressed = event.FilterAs<MousePressedEvent>())
				{
					spriteComp.color = buttonComp.pressedColor;
					buttonComp.heldDown = true;
					return true;
				}
				else if (auto released = event.FilterAs<MouseReleasedEvent>())
				{
					/* if (buttonComp.heldDown && buttonComp.hovering)
					{
						entity.GetComponent<Button>().callback();
					} */

					if (buttonComp.hovering)
						spriteComp.color = buttonComp.hoverColor;
					else
						spriteComp.color = buttonComp.baseColor;
					buttonComp.heldDown = false;
					return true;
				}
				else if (auto enterEvent = event.FilterAs<MouseHoverEnterEvent>())
				{
					if (!buttonComp.heldDown)
						spriteComp.color = buttonComp.hoverColor;
					buttonComp.hovering = true;
					return true;
				}
				else if (auto exitEvent = event.FilterAs<MouseHoverExitEvent>())
				{
					if (!buttonComp.heldDown)
						spriteComp.color = buttonComp.baseColor;
					buttonComp.hovering = false;
					return true;
				}

				if (MouseMovedEvent* move = event.FilterAs<MouseMovedEvent>())
				{
					UITransform& handle = entity.GetUITransform();
					float pos = entity.GetScene()->ScreenToWorldPos({ move->GetPosX(), 0 }).x;
					float value = glm::clamp(handle.GetPrimaryXFromWorldPos(pos), -0.5f, 0.5f);

					handle.SetXPosValue(value);
					sliderComp.value = value + 0.5f;
					sliderComp.callback(sliderComp.value);

					return true;
				}

				return false;
			};
			auto sliderBaseCallback = [](Entity entity, Event& event) {
				
				if (MousePressedEvent* pressed = event.FilterAs<MousePressedEvent>())
				{
					auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
					auto& sliderComp = entity.GetComponent<Slider>();

					UITransform& handle = entity.GetChild(0).GetUITransform();
					float pos = entity.GetScene()->ScreenToWorldPos(Input::MousePos()).x;
					float value = glm::clamp(handle.GetPrimaryXFromWorldPos(pos), -0.5f, 0.5f);

					handle.SetXPosValue(value);
					sliderComp.value = value + 0.5f;
					sliderComp.callback(sliderComp.value);

					return true;
				}

				return false;
			};

			UIEntityParams handleParams = UIEntityParams(UITransform::XDriver::Center, 0.5, UITransform::WidthDriver::AspectRatio, 1, 
				UITransform::YDriver::Center, 0, UITransform::HeightDriver::Relative, 1.5, params.sortingLayer, params.subSorting + 1);
			Entity sliderHandle = _slider.GetScene()->AddUIEntity("Handle", _slider, handleParams);

			sliderHandle.AddComponent<Button>(Button(glm::vec4{ 1 }, glm::vec4{ 0.9f }, glm::vec4{ 0.7f }, []() {}));
			sliderHandle.AddComponent<SpriteRendererComponent>(glm::vec4{ 1 });
			_slider.AddComponent<Slider>(std::forward<Slider>(sliderComponent));
			_slider.AddComponent<SpriteRendererComponent>(sliderComponent.baseColor);


			_buttonComp = &sliderHandle.GetComponent<Button>();
			_handleSpriteComp = &sliderHandle.GetComponent<SpriteRendererComponent>();
			_baseSpriteComp = &_slider.GetComponent<SpriteRendererComponent>();
			_sliderComp = &_slider.GetComponent<Slider>();

			_sliderComp->ActivateUIElement(_slider, sliderBaseCallback);
			_buttonComp->ActivateUIElement(sliderHandle, handleCallback);

		}

	}
}
