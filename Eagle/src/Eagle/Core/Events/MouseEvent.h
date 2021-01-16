#pragma once
#include "Event.h"

namespace Egl {
	class MouseButtonEvent : public Event {
	public:
		inline int GetButton() const { return mButton; }
		EVENT_CLASS_GATEGORY(EventGategoryMouse | EventGategoryInput);
	protected:
		MouseButtonEvent(int button) : mButton(button) {}
		int mButton;
	};

	class MousePressedEvent : public MouseButtonEvent {
	public:
		MousePressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream str;
			str << "MousePressed: " << mButton;
			return str.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class MouseReleasedEvent : public MouseButtonEvent {
	public:
		MouseReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream str;
			str << "MouseReleased: " << mButton;
			return str.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(int posX, int posY)
			: mPosX(posX), mPosY(posY) {}

		inline int GetPosX() const { return mPosX; }
		inline int GetPosY() const { return mPosY; }

		std::string ToString() const override {
			std::stringstream str;
			str << "MouseMoved: Position (" << mPosX << ", " << mPosY << ")";
			return str.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_GATEGORY(EventGategoryMouse | EventGategoryInput);
	protected:
		int mPosX, mPosY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(int scrollX, int scrollY)
			: mScrollX(scrollX), mScrollY(scrollY) {}

		inline int GetScrollXOffset() const { return mScrollX; }
		inline int GetScrollYOffset() const { return mScrollY; }

		std::string ToString() const override {
			std::stringstream str;
			str << "MouseScrolled: Scrolling position (" << mScrollX << ", " << mScrollY << ")";
			return str.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_GATEGORY(EventGategoryMouse | EventGategoryInput);
	protected:
		int mScrollX, mScrollY;
	};
}
