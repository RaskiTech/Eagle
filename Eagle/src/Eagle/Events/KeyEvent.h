#pragma once
#include "Event.h"

namespace Egl {
	class EAGLE_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_GATEGORY(EventGategoryKeyboard | EventGategoryInput);
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class EAGLE_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keykode, int repeatCount)
			: KeyEvent(keykode), repeatCount(repeatCount) {}

		EVENT_CLASS_TYPE(KeyPressed);

		std::string ToString() const override {
			std::stringstream str;
			str << "KeyPressed: " << m_KeyCode << ", repeatCount: " << repeatCount;
			return str.str();
		}
	protected:
		bool repeatCount;
	};

	class EAGLE_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keykode)
			: KeyEvent(keykode) {}
		 
		EVENT_CLASS_TYPE(KeyReleased);

		std::string ToString() const override {
			std::stringstream str;
			str << "KeyReleased: " << m_KeyCode;
			return str.str();
		}
	};
}
