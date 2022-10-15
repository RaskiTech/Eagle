#pragma once

#include "Eagle/Core/Core.h"

namespace Egl {

	enum class EventType {
		None = 0,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		KeyPressed, KeyReleased, KeyTyped,
		WindowClose, WindowRezise, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender
	};

	enum class EventGategory {
		None = 0,
		Application = BIT(0),
		Input       = BIT(1),
		Keyboard    = BIT(2),
		Mouse       = BIT(3),
		MouseButton = BIT(4)
	};

	class Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetGategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName();  }

		inline bool IsInGategory(EventGategory category) {
			return GetGategoryFlags() & (int)category;
		}
		inline bool IsHandled() const { return handled; }

		bool handled = false;
	};

	class EventDispatcher {
		template <typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : mEvent(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (mEvent.GetEventType() == T::GetStaticType()) {
				mEvent.handled = func(*(T*)&mEvent);
				return true;
			}
			return false;
		}
	private:
		Event& mEvent;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

#define EVENT_CLASS_TYPE(type) \
	static EventType GetStaticType() {return EventType::##type;} \
	virtual EventType GetEventType() const override {return GetStaticType();} \
	virtual const char* GetName() const override {return #type; }
#define EVENT_CLASS_GATEGORY(category) \
		virtual int GetGategoryFlags() const override {return (int)(category);}
}
