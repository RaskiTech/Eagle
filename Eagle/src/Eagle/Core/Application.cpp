#include "EaglePCH.h"
#include "Application.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"

#include "GLFW/glfw3.h"

#include "Eagle/Rendering/VertexArray.h"
namespace Egl {

#define EAGLE_BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	Application::Application()
	{
		EAGLE_PROFILE_FUNCTION();

		mInstance = this;

		mWindow = Window::Create();
		mWindow->SetEventCallback(EAGLE_BIND_EVENT_FUNC(OnEvent));

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);
	}

	Application::~Application() {
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e) {
		EAGLE_PROFILE_SCOPE(e.GetName());
		EventDispatcher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(EAGLE_BIND_EVENT_FUNC(OnWindowClose));
		dispacher.Dispatch<WindowResizeEvent>(EAGLE_BIND_EVENT_FUNC(OnWindowResize));

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
		EAGLE_PROFILE_FUNCTION();
		mLayerStack.AddLayer(layer);
		layer->OnAttach();
	}
	void Application::AddOverlay(Layer* layer) {
		EAGLE_PROFILE_FUNCTION();
		mLayerStack.AddOverlay(layer);
		layer->OnAttach();
	}
	void Application::RemoveLayer(Layer* layer) {
		EAGLE_PROFILE_FUNCTION();
		layer->OnDetach();
		mLayerStack.RemoveLayer(layer);
	}
	void Application::RemoveOverlay(Layer* layer) {
		EAGLE_PROFILE_FUNCTION();
		layer->OnDetach();
		mLayerStack.RemoveOverlay(layer);
	}

	void Application::Run() {
		EAGLE_PROFILE_FUNCTION();
		while (mRunning) {
			EAGLE_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Time::SetTime(time, time - mLastFrameTime);
			mLastFrameTime = time;

			if (!mMinimized) {
				{
					EAGLE_PROFILE_SCOPE("Layer OnUpdates");
					// Update the layers
					for (Layer* layer : mLayerStack)
						if (layer->IsActive()) {
							EAGLE_PROFILE_SCOPE(layer->GetName().c_str());
							layer->OnUpdate();
						}
				}

				{
					EAGLE_PROFILE_SCOPE("ImGui update");
					mImGuiLayer->Begin();
					for (Layer* layer : mLayerStack)
						if (layer->IsActive())
							layer->OnImGuiRender();
					mImGuiLayer->End();
				}
			}

			mWindow->OnUpdate();
		}

		Renderer::Shutdown();
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		mRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e) {
		EAGLE_PROFILE_FUNCTION();
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