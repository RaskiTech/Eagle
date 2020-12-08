#pragma once
#include "Event.h"
// WindowFocus, WindowLostFocus, WindowMoved,

namespace Egl {
	class EAGLE_API WindowResizeEvent : public Event {
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

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(WindowRezise);
	protected:
		unsigned int mWidth, mHeight;
	};

	class EAGLE_API WindowMovedEvent : public Event {
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

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(WindowRezise);
	protected:
		unsigned int mPosX, mPosY;
	};

	class EAGLE_API WindowFocusEvent : public Event {
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(WindowFocus);
	};

	class EAGLE_API WindowLostFocusEvent : public Event {
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(WindowLostFocus);
	};

	class EAGLE_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(WindowClose);
	};

	class EAGLE_API AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(AppTick);
	};

	class EAGLE_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(AppRender);
	};

	class EAGLE_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_GATEGORY(EventGategoryApplication);
		EVENT_CLASS_TYPE(AppUpdate);
	};
}
