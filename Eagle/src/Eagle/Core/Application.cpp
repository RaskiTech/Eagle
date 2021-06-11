#include <EaglePCH.h>
#include <EagleBuildSettings.h>
#include "Application.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Core/Random.h"
#include "Eagle/Core/GameLayer.h"
#include "Eagle/Debug/EditorLayer.h"
#include "Eagle/Core/Time.h"
#include "UniqueID.h"

// This Commit:
// Engine sliders for UI now keep the UI at the same place when changing between sides and transform

namespace Egl {
	std::uniform_int_distribution<std::mt19937::result_type> Random::sDistribution;
	std::mt19937 Random::sRandomizer;
	uint32_t UniqueID::mCurrentFrameID;

	Application* Application::mInstance = nullptr;

#define EAGLE_BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application(const std::string& name) {
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
		mViewportSize = { (float)mWindow->GetWidth(), (float)mWindow->GetHeight() };
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

		GetGameLayer()->DistributeEvent(e);
	}

	void Application::Run() {
		EAGLE_PROFILE_FUNCTION();
		while (mRunning) {
			EAGLE_PROFILE_SCOPE("RunLoop");

			float time = mWindow->GetTime();
			Time::SetTime(time, time - mLastFrameTime);
			mLastFrameTime = time;

			UniqueID::ResetFrameIDs();

			if (!mMinimized) {
				{
					EAGLE_PROFILE_SCOPE("Layer OnUpdates");
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
				}
				{
					EAGLE_PROFILE_SCOPE("ImGui update");
					mImGuiLayer->Begin();
					for (Layer* layer : mLayerStack)
						if (layer->IsActive())
							layer->OnImGuiRender();
					mImGuiLayer->End();
#endif
				}
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
		mViewportSize = { (float)e.GetWidth(), (float)e.GetHeight() };
		mGameLayer->GetActiveScene()->SetViewportAspectRatio(mViewportSize.x / mViewportSize.y);
#endif
		return false;
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

	const glm::vec2& Application::GetSceneScreenOffset() const {
#if EAGLE_EDITOR
		return mScenePanelOffset;
#else
		return { 0, 0 };
#endif
	}
	glm::vec2 Application::WindowPixelToScenePixelSpace(const glm::vec2& point) const {
#if EAGLE_EDITOR
		return point - mScenePanelOffset;
#else
		return point;
#endif
	}
	glm::vec2 Application::ScenePixelToWindowPixelSpace(const glm::vec2& point) const {
#if EAGLE_EDITOR
		return point + mScenePanelOffset;
#else
		return point;
#endif
	}
	int Application::WindowPixelToScenePixelSpaceX(int point) const {
#if EAGLE_EDITOR
		return point - (int)mScenePanelOffset.x;
#else
		return point;
#endif
	}
	int Application::WindowPixelToScenePixelSpaceY(int point) const {
#if EAGLE_EDITOR
		return point - (int)mScenePanelOffset.y;
#else
		return point;
#endif
	}
}