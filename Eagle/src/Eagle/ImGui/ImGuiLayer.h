#pragma once

#include "Eagle/Layers/Layer.h"
#include "Eagle/Events/KeyEvent.h"
#include "Eagle/Events/MouseEvent.h"
#include "Eagle/Events/ApplicationEvent.h"

namespace Egl {
	class EAGLE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMousePressedEvent(MousePressedEvent& e);
		bool OnMouseReleasedEvent(MouseReleasedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEventEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		float mTime = 0.0f;
	};
}
