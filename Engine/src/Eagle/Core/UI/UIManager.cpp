#include <EaglePCH.h>
#include "UIManager.h"
#include "Eagle/ECS/Entity.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/Core/Events/UIEvent.h"
#include "Eagle/Core/Events/MouseEvent.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/Core/Keycodes.h"


#define PRODCAST_UI_EVENT_TO_SELECTED(event) _elements[_selectedEntity](Entity(_selectedEntity, _selectedEntityScene), event)


namespace Egl
{
	UIElement::UIElement()
	{

	}

	UIElement::~UIElement()
	{

	}

	void UIElement::ActivateUIElement(Entity entityThatHasComponent, std::function<bool(Entity, Event&)> UIEventCallback)
	{
		ClearUIElement();

		_entity = (entt::entity)entityThatHasComponent.GetID();
		_scene = entityThatHasComponent.GetScene();

		entityThatHasComponent.GetScene()->SubscribeToEvents(entityThatHasComponent, std::function<bool(Entity, Event&)>(std::bind(&UIElement::OnEvent, this, std::placeholders::_1, std::placeholders::_2)));
		_scene->SubscribeToEnterExitEvents(entityThatHasComponent, std::function<bool(Entity, Event&)>(std::bind(&UIElement::OnEvent, this, std::placeholders::_1, std::placeholders::_2)));
		entityThatHasComponent.GetScene()->GetUIManager().AddUIElement(entityThatHasComponent, UIEventCallback);
	}

	void UIElement::ClearUIElement()
	{
		if (_entity == entt::null || _scene == nullptr)
			return;

		_scene->GetUIManager().RemoveUIElement(Entity(_entity, _scene));
		LOG("TODO: Implement unsubscribing of Eagle events");
	}

	bool UIElement::OnEvent(Entity e, Event& event)
	{
		return e.GetScene()->GetUIManager().ForwardElementEvent(e, event);
	}



	//////////////////////////
	////// UI Manager ////////
	//////////////////////////


	UIManager::~UIManager()
	{
		DeselectElement();
	}

	bool UIManager::PreEventDistribution(Event& e)
	{
		if (_selectedState == SelectedState::Pressed)
		{
			if (e.FilterAs<MouseReleasedEvent>() && e.FilterAs<MouseReleasedEvent>()->GetButton() == EGL_UI_SELECT_BUTTON)
			{
				_selectedState = SelectedState::Selected;
			}

			if (ShouldForwardEventToSelected(e))
				return PRODCAST_UI_EVENT_TO_SELECTED(e);
		}

		
		return false;
	}
	bool UIManager::ForwardElementEvent(Entity entity, Event& e)
	{
		if (MousePressedEvent* pressed = e.FilterAs<MousePressedEvent>())
		{
			if (pressed->GetButton() == EGL_UI_SELECT_BUTTON)
				SelectElement(entity);

			return true;
		}
		else if (MouseHoverEnterEvent* enter = e.FilterAs<MouseHoverEnterEvent>())
		{
			_elements[(entt::entity)entity.GetID()](entity, *enter);
		}
		else if (MouseHoverExitEvent* exit = e.FilterAs<MouseHoverExitEvent>())
		{
			_elements[(entt::entity)entity.GetID()](entity, *exit);
		}

		return false;
	}

	void UIManager::SelectElement(Entity entity)
	{
		if (_selectedEntity != entity)
			DeselectElement();

		_selectedEntity = (entt::entity)entity.GetID();
		_selectedEntityScene = entity.GetScene();

		auto& func = _elements[_selectedEntity];
		PRODCAST_UI_EVENT_TO_SELECTED(MousePressedEvent(EGL_UI_SELECT_BUTTON));

		if (_selectedState == SelectedState::Unselected)
			PRODCAST_UI_EVENT_TO_SELECTED(UIElementSelected());


		_selectedState = SelectedState::Pressed;
	}

	void UIManager::DeselectElement()
	{
		if (_selectedEntity == entt::null)
			return;

		if (_selectedState == SelectedState::Pressed)
			PRODCAST_UI_EVENT_TO_SELECTED(MouseReleasedEvent(EGL_UI_SELECT_BUTTON));

		PRODCAST_UI_EVENT_TO_SELECTED(UIElementDeselected());
		_selectedState = SelectedState::Unselected;
		_selectedEntity = entt::null;
		_selectedEntityScene = nullptr;
	}

	void UIManager::AddUIElement(Entity entity, std::function<bool(Entity, Event&)> UIEventCallback)
	{
		_elements.insert(std::pair((entt::entity)entity.GetID(), UIEventCallback));
	}

	void UIManager::RemoveUIElement(Entity entity)
	{
		_elements.erase((entt::entity)entity.GetID());
	}

}
