#pragma once

#include "Eagle/Core/Window.h"
#include <GLFW/glfw3.h>
#include "Eagle/Rendering/RenderingContext.h"

namespace Egl {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		void Render() override;

		inline uint32_t GetWidth() const override { return mData.width; }
		inline uint32_t GetHeight() const override { return mData.height; }
		inline float GetTime() const override { return (float)glfwGetTime(); }
		virtual uint32_t GetPositionX() const override { return mData.posX; }
		virtual uint32_t GetPositionY() const override { return mData.posY; }

		void SetClipboard(const std::string& text) const override { glfwSetClipboardString(mWindow, text.c_str()); }
		std::string GetClipboard() const override { return std::string(glfwGetClipboardString(mWindow)); }

		inline void SetEventCallback(const EventCallbackFn& callback) override { mData.EventCallback = callback; }
		bool IsVSync() const override;
		void SetVSync(bool enabled) override;

		inline virtual void* NativeWindow() const override { return mWindow; }

	private:
		struct WindowData {
			std::string Title;
			unsigned int posX = 0, posY = 0;
			unsigned int width = 0, height = 0;

			bool VSync = true;
			EventCallbackFn EventCallback;
			inline std::string ToString() { return Title; }
		};
		GLFWwindow* mWindow;
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();


		WindowData mData;
		RenderingContext* mContext;
	};
}