#pragma once

#include <EaglePCH.h>
#include "Eagle/Events/Event.h"

namespace Egl {
	struct WindowProps {
		std::string Title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title,
			uint32_t width = 1280, uint32_t height = 720)
			: Title(title), width(width), height(height) {}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* NativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Scope<Window> Create(const WindowProps& props);
	};
}