#pragma once
#include "Dependencies/Entt.h"
#include "Eagle/Core/Events/UIEvent.h"
#include "Eagle/Core/Events/MouseEvent.h"
#include <vector>

namespace Egl
{
	#define EGL_UI_SELECT_BUTTON EGL_MOUSE_BUTTON_LEFT
	
	class Entity;
	class Scene;

	class UIElement
	{
	public:
		UIElement();
		~UIElement();

		void ActivateUIElement(Entity entityThatHasComponent, std::function<bool(Entity, Event&)> UIEventCallback);
		void ClearUIElement();

		bool OnEvent(Entity e, Event& event);

		entt::entity _entity = entt::null;
		Scene* _scene = nullptr;
	};

	class UIManager
	{
	public:
		UIManager() = default;
		~UIManager();

		void AddUIElement(Entity entity, std::function<bool(Entity, Event&)> UIEventCallback);
		void RemoveUIElement(Entity entity);
		

		bool PreEventDistribution(Event& e);
		bool ForwardElementEvent(Entity entity, Event& e);
		void SelectElement(Entity entity);
		void DeselectElement();

	private:
		bool ShouldForwardEventToSelected(Event& e) {
			return e.FilterAs<MouseMovedEvent>() ||
				   e.FilterAs<MouseScrolledEvent>() ||
				  (e.FilterAs<MousePressedEvent>() && e.FilterAs<MousePressedEvent>()->GetButton() == EGL_UI_SELECT_BUTTON) ||
				  (e.FilterAs<MouseReleasedEvent>() && e.FilterAs<MouseReleasedEvent>()->GetButton() == EGL_UI_SELECT_BUTTON);
		}

		enum class SelectedState
		{
			Unselected = 0,
			Pressed,
			Selected,
		};

		entt::entity _selectedEntity = entt::null;
		Scene* _selectedEntityScene = nullptr;
		SelectedState _selectedState = SelectedState::Unselected;

		std::unordered_map<entt::entity, std::function<bool(Entity, Event&)>> _elements = std::unordered_map<entt::entity, std::function<bool(Entity, Event&)>>();
	};
}
