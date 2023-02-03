#pragma once
#include "Event.h"
#include "MouseEvent.h"
// UIElementPressed, UIElementReleased, UIElementHovered, UIElementUnhovered, UIElementSelected, UIElementDeselected

namespace Egl {

	class UIEvent : public Event
	{

	};

	class UIElementSelected : public UIEvent {
	public:
		UIElementSelected() {}

		std::string ToString() const override {
			std::stringstream str;
			str << "UIElementSelected";
			return str.str();
		}

		EVENT_CLASS_GATEGORY(EventGategory::UI);
		EVENT_CLASS_TYPE(UIElementSelected);
	protected:

	};

	class UIElementDeselected : public UIEvent {
	public:
		UIElementDeselected() {}

		std::string ToString() const override {
			std::stringstream str;
			str << "UIElementDeselected";
			return str.str();
		}

		EVENT_CLASS_GATEGORY(EventGategory::UI);
		EVENT_CLASS_TYPE(UIElementDeselected);
	protected:

	};
}
