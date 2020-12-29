#include "EaglePCH.h"
#include "Application.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"

#include "GLFW/glfw3.h"

namespace Egl {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	Application::Application()
	{
		mInstance = this;

		mWindow = Scope<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
		dispacher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OnWindowResize));

		for (auto layer = mLayerStack.end(); layer != mLayerStack.begin();) {
			layer--;
			if ((*layer)->IsActive()) {
				(*layer)->OnEvent(e);
				if (e.IsHandled())
					break;
			}
		}
	}

	void Application::AddLayer(Layer* layer) {
		mLayerStack.AddLayer(layer);
		layer->OnAttach();
	}
	void Application::AddOverlay(Layer* layer) {
		mLayerStack.AddOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run() {
		while (mRunning) {
			float time = (float)glfwGetTime();
			Time::SetTime(time, time - mLastFrameTime);
			mLastFrameTime = time;

			if (!mMinimized) {
				// Update the layers
				for (Layer* layer : mLayerStack)
					if (layer->IsActive())
						layer->OnUpdate();
			}

			// ImGui update
			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				if (layer->IsActive())
					layer->OnImGuiRender();
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}

		Renderer::Shutdown();
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		mRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e) {
		// If the window is minimized, don't run the main loop
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			mMinimized = true;
			return false;
		}
		mMinimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}