#pragma once

#include "Eagle/Window.h"
#include <GLFW/glfw3.h>

namespace Egl {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return mData.width; }
		inline unsigned int GetHeight() const override { return mData.height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { mData.EventCallback = callback; }
		bool IsVSync() const override;
		void SetVSync(bool enabled) override;

		inline virtual void* NativeWindow() const override { return mWindow; }
	private:
		GLFWwindow* mWindow;
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		struct WindowData {
			std::string Title;
			unsigned int width, height;
			bool VSync;
			EventCallbackFn EventCallback;
			inline std::string ToString() { return Title; }
		};
		WindowData mData;
	};
}