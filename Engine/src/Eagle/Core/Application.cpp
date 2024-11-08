#include <EaglePCH.h>
#include <Eagle/Core/Core.h>
#include "Application.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"
#include "Eagle/Core/Random.h"
#include "Eagle/Core/GameLayer.h"
#include "Eagle/Core/Serialization.h"
#include "Eagle/Debug/EditorLayer.h"
#include "Eagle/Core/Time.h"
#include "Eagle/Core/Audio.h"
#include "Eagle/Core/Python/PythonEmbedding.h"

// This Commit:
// Stability for playing audio

namespace Egl {
	std::uniform_int_distribution<std::mt19937::result_type> Random::sDistribution;
	std::mt19937 Random::sRandomizer;
	std::string SaveData::persistantDataPath;

	Application* Application::mInstance = nullptr;

#define EAGLE_BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() : pythonEmbedding() {
		EAGLE_PROFILE_FUNCTION();

		mInstance = this;

		mWindow = Window::Create(WindowProps(APPLICATION_NAME, SCREEN_WIDTH, SCREEN_HEIGHT));
		mWindow->SetEventCallback(EAGLE_BIND_EVENT_FUNC(OnEvent));

		{
			EAGLE_PROFILE_SCOPE("Miscellaneous inits");
			Renderer::Init();
			Random::Init();
			SaveData::Init();
			AudioPlayer::Init();
		}

		// On editor this will be overwritten on the next frame
		mSceneWindowSize = { (float)mWindow->GetWidth(), (float)mWindow->GetHeight() };

		mGameLayer = new GameLayer();
		mGameLayer->OnAttach();

#if EAGLE_ENABLE_IMGUI
		mImGuiLayer = new ImGuiLayer();
		mImGuiLayer->OnAttach();
#endif
#if EAGLE_EDITOR
		mEditorLayer = new EditorLayer();
		mEditorLayer->OnAttach();
#endif
	}

	Application::~Application() {
		EAGLE_PROFILE_FUNCTION();

		if (mEditorLayer)
			delete mEditorLayer;
		if (mImGuiLayer)
			delete mImGuiLayer;
		if (mGameLayer)
			delete mGameLayer;

		AudioPlayer::Close();
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e) {
		EAGLE_PROFILE_SCOPE(e.GetName());
		EventDispatcher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(EAGLE_BIND_EVENT_FUNC(OnWindowClose));
		dispacher.Dispatch<WindowResizeEvent>(EAGLE_BIND_EVENT_FUNC(OnWindowResize));

		if (mImGuiLayer)
			mImGuiLayer->OnEvent(e);
		if (!e.handled)
			mGameLayer->OnEvent(e);
	}

	void Application::Run() {
		EAGLE_PROFILE_FUNCTION();
		while (mRunning) {
			EAGLE_PROFILE_SCOPE("RunLoop");

			{
				float time = mWindow->GetTime();
				Time::SetTime(time, time - mLastFrameTime);
				mLastFrameTime = time;
			}

			if (!mMinimized) {

				{
					EAGLE_PROFILE_SCOPE("Updating layers");

	#if EAGLE_EDITOR
					mEditorLayer->OnUpdate();
					mGameLayer->OnUpdate(false);
	#else
					mGameLayer->OnUpdate(true);
	#endif

	#if EAGLE_ENABLE_IMGUI
					{
						mImGuiLayer->Begin();

		#if EAGLE_EDITOR
						mEditorLayer->OnImGuiRender();
		#endif
						mGameLayer->OnImGuiRender();

						mImGuiLayer->End();
					}
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
		mSceneWindowSize = { (float)e.GetWidth(), (float)e.GetHeight() };
		Assets::GetScene(mGameLayer->GetActiveScene())->ChangeCameraAspectRatios(mSceneWindowSize.x / mSceneWindowSize.y);
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