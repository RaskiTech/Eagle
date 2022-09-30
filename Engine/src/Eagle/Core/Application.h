#pragma once
#include "Core.h"
#include "Window.h"
#include "Eagle/Core/GameLayer.h"
#include "Eagle/Debug/ImGui/ImGuiLayer.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/Core/Events/ApplicationEvent.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/Core/Python/PythonEmbedding.h"

namespace Egl {

	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void Close();
		void OnEvent(Event& e);

		inline ImGuiLayer* GetImGuiLayer() const { return mImGuiLayer; }
		inline GameLayer* GetGameLayer() const { return mGameLayer; }
		inline EditorLayer* GetEditorLayer() const { return mEditorLayer; }
		inline Window& GetWindow() const { return *mWindow; }
		inline const PythonEmbedding& GetPython() const { return pythonEmbedding; }

		const glm::vec2& GetSceneWindowSize() const { return mSceneWindowSize; }
		const glm::vec2& GetSceneScreenOffset() const;

		glm::vec2 WindowPixelToScenePixelSpace(const glm::vec2& point) const;
		glm::vec2 ScenePixelToWindowPixelSpace(const glm::vec2& point) const;
		int WindowPixelToScenePixelSpaceX(int point) const;
		int WindowPixelToScenePixelSpaceY(int point) const;

		static inline Application& Get() { return *mInstance; }
	protected:
		void SetSceneWindowSize(const glm::vec2& size) { mSceneWindowSize = size; }
		void SetSceneScreenOffset(const glm::vec2& offset) { mScenePanelOffset = offset; }
		friend class EditorLayer;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		// Keep this at the top because it has the glfw context
		Scope<Window> mWindow;
		PythonEmbedding pythonEmbedding;

		ImGuiLayer* mImGuiLayer = nullptr;
		EditorLayer* mEditorLayer = nullptr;
		GameLayer* mGameLayer = nullptr;

		glm::vec2 mSceneWindowSize = { 0, 0 };  // Tells the window size
		glm::vec2 mScenePanelOffset = { 0, 0 }; // Only in edit mode, can always call GetSceneWindowOffset() though;

		bool mRunning = true;
		bool mMinimized = false;
		static Application* mInstance;

		float mLastFrameTime = 0;
	};
}
