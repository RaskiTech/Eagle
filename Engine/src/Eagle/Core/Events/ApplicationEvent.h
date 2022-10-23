#pragma once
#include "Event.h"
// WindowFocus, WindowLostFocus, WindowMoved,

namespace Egl {
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: mWidth(width), mHeight(height) {}

		inline unsigned int GetWidth() const { return mWidth; }
		inline unsigned int GetHeight() const { return mHeight; }

		std::string ToString() const override {
			std::stringstream str;
			str << "WindowRezizeEvent (" << mWidth << ", " << mHeight << ")";
			return str.str();
		}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(WindowRezise);
	protected:
		unsigned int mWidth, mHeight;
	};

	class WindowMovedEvent : public Event {
	public:
		WindowMovedEvent(unsigned int posX, unsigned int posY)
			: mPosX(posX), mPosY(posY) {}

		inline unsigned int GetPosX() const { return mPosX; }
		inline unsigned int GetPosY() const { return mPosY; }

		std::string ToString() const override {
			std::stringstream str;
			str << "WindowMovedEvent: position (" << mPosX << ", " << mPosY << ")";
			return str.str();
		}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(WindowRezise);
	protected:
		unsigned int mPosX, mPosY;
	};

	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(WindowFocus);
	};

	class WindowLostFocusEvent : public Event {
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(WindowLostFocus);
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(WindowClose);
	};

	// As of now, we don't send AppTick, AppRender or AppUpdate events
	class AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(AppTick);
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(AppRender);
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_GATEGORY(EventGategory::Application);
		EVENT_CLASS_TYPE(AppUpdate);
	};
}
