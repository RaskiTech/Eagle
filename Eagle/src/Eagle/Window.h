#pragma once

#include "EaglePCH.h"

#include "Eagle/Core.h"
#include "Eagle/Events/Event.h"

namespace Egl {
	struct WindowProps {
		std::string Title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Eagle Engine",
			unsigned int width = 1250, unsigned int height = 750)
			: Title(title), width(width), height(height) {}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void* NativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}