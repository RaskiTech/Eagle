#pragma once

#include <EaglePCH.h>
#include "Eagle/Core/Events/Event.h"
#include <glm/glm.hpp>

namespace Egl {
	struct WindowProps {
		std::string Title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title,
			uint32_t width = 1400, uint32_t height = 800)
			: Title(title), width(width), height(height) {}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual void Render() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetPositionY() const = 0;
		virtual uint32_t GetPositionX() const = 0;

		virtual float GetTime() const = 0;

		virtual void* NativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Scope<Window> Create(const WindowProps& props);
	};
}