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
// Text options now work & editor panel for textComponent

namespace Egl {
	std::uniform_int_distribution<std::mt19937::result_type> Random::sDistribution;
	std::mt19937 Random::sRandomizer;
	uint32_t UniqueID::mCurrentFrameID;

	Application* Application::mInstance = nullptr;

#define EAGLE_BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() {
		EAGLE_PROFILE_FUNCTION();

		mInstance = this;

		mWindow = Window::Create(WindowProps(APPLICATION_NAME, SCREEN_WIDTH, SCREEN_HEIGHT));
		mWindow->SetEventCallback(EAGLE_BIND_EVENT_FUNC(OnEvent));

		Renderer::Init();
		Random::Init();

		// On editor this will be overwritten on the next frame
		mSceneWindowSize = { (float)mWindow->GetWidth(), (float)mWindow->GetHeight() };

		mGameLayer = new GameLayer();
		mGameLayer->OnAttach();

#if EAGLE_EDITOR
		mImGuiLayer = new ImGuiLayer();
		mImGuiLayer->OnAttach();

		mEditorLayer = new EditorLayer();
		mEditorLayer->OnAttach();
#else
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

		mGameLayer->DistributeEvent(e);
	}

	void Application::Run() {
		EAGLE_PROFILE_FUNCTION();
		while (mRunning) {
			EAGLE_PROFILE_SCOPE("RunLoop");

			// Utility handling
			{
				float time = mWindow->GetTime();
				Time::SetTime(time, time - mLastFrameTime);
				mLastFrameTime = time;

				UniqueID::ResetFrameIDs();
			}

			if (!mMinimized) {

				{
					EAGLE_PROFILE_SCOPE("Updating layers");

	#if EAGLE_EDITOR
					mEditorLayer->OnUpdate();
					mGameLayer->OnUpdate(false);

					{
						EAGLE_PROFILE_SCOPE("ImGui update");
						mImGuiLayer->Begin();

						mEditorLayer->OnImGuiRender();
						mGameLayer->OnImGuiRender();

						mImGuiLayer->End();
					}
	#else // EAGLE_EDITOR
					mGameLayer->OnUpdate(true);
	#endif // EAGLE_EDITOR
					
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
		mSceneWindowSize = { (float)e.GetWidth(), (float)e.GetHeight() };
		mGameLayer->GetActiveScene()->SetViewportAspectRatio(mSceneWindowSize.x / mSceneWindowSize.y);
#endif

		return false;
	}

	void Application::Close() {
		mRunning = false;
	}

	const glm::vec2& Application::GetSceneScreenOffset() const {
		return mScenePanelOffset;
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