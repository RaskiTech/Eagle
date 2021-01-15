#include <EaglePCH.h>
#include "Application.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Core/Random.h"
#include "Eagle/Scripting/GameLayer.h"
#include "EditorLayer/EditorLayer.h"

#include <GLFW/glfw3.h>
#include "Eagle/Rendering/VertexArray.h"

// This is the only file that does different things depending on this
#define EAGLE_EDITOR 1


namespace Egl {
	std::uniform_int_distribution<std::mt19937::result_type> Random::sDistribution;
	std::mt19937 Random::sRandomizer;
	Application* Application::mInstance = nullptr;

#define EAGLE_BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application(const std::string& name)
	{
		EAGLE_PROFILE_FUNCTION();

		mInstance = this;

		mWindow = Window::Create(name);
		mWindow->SetEventCallback(EAGLE_BIND_EVENT_FUNC(OnEvent));

		Renderer::Init();
		Random::Init();

#if EAGLE_EDITOR
		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);
#endif
		mGameLayer = new GameLayer();
		AddLayer(mGameLayer);
#if EAGLE_EDITOR
		mEditorLayer = new EditorLayer();
		AddLayer(mEditorLayer);
#else
		mGameLayer->GetActiveScene()->SetViewportAspectRatio((float)mWindow->GetWidth() / mWindow->GetHeight());
#endif
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
	void Application::Close() {
		mRunning = false;
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
					LOG_INFO("START");
#if EAGLE_EDITOR
					mEditorLayer->PreUpdate();
#endif

					for (Layer* layer : mLayerStack)
						if (layer->IsActive()) {
							EAGLE_PROFILE_SCOPE(layer->GetName().c_str());
							layer->OnUpdate();
						}

#if EAGLE_EDITOR
					mEditorLayer->PostUpdate();
#endif
					LOG_INFO("STOP");
				}
#if EAGLE_EDITOR
				{
					EAGLE_PROFILE_SCOPE("ImGui update");
					mImGuiLayer->Begin();
					for (Layer* layer : mLayerStack)
						if (layer->IsActive())
							layer->OnImGuiRender();
					mImGuiLayer->End();
				}
#endif
			}

			mWindow->OnUpdate();
		}
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		Close();
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
#if !EAGLE_EDITOR
		mGameLayer->GetActiveScene()->SetViewportAspectRatio((float)e.GetWidth() / e.GetHeight());
#endif
		return false;
	}
}