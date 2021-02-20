#pragma once
#include <glm/glm.hpp>
#include "Core.h"
#include "Window.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/Core/Events/ApplicationEvent.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/Core/Layers/LayerStack.h"
#include "Eagle/Debug/ImGui/ImGuiLayer.h"
#include "Eagle/Core/GameLayer.h"

namespace Egl {

	class Application
	{
	public:
		Application(const std::string& name = "Eagle App");
		virtual ~Application();
		void Run();
		void Close();
		void OnEvent(Event& e);

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* layer);
		void RemoveLayer(Layer* layer);
		void RemoveOverlay(Layer* layer);

		ImGuiLayer* GetImGuiLayer() const { return mImGuiLayer; }
		GameLayer* GetGameLayer() const { return mGameLayer; }
		inline Window& GetWindow() const { return *mWindow; }

		const glm::vec2& GetViewportSize() const { return mViewportSize; }
		const glm::vec2& GetSceneWindowSize() const;
		const glm::vec2& GetSceneScreenOffset() const;

		glm::vec2 WindowPixelToScenePixelSpace(const glm::vec2& point) const;
		glm::vec2 ScenePixelToWindowPixelSpace(const glm::vec2& point) const;

		static inline Application& Get() { return *mInstance; }
	protected:
		void SetViewportSize(const glm::vec2& size) { mViewportSize = size; }
		void SetScenePanelSize(const glm::vec2& size) { mScenePanelSize = size; }
		void SetSceneScreenOffset(const glm::vec2& offset) { mScenePanelOffset = offset; }
		friend class EditorLayer;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		// Keep this at top because it has the glfw context
		Scope<Window> mWindow;

		LayerStack mLayerStack;

		ImGuiLayer* mImGuiLayer;
		EditorLayer* mEditorLayer;
		GameLayer* mGameLayer;

		glm::vec2 mViewportSize = { 0, 0 };
		glm::vec2 mScenePanelSize = { 0, 0 };   // Only in edit mode, can always call GetSceneWindowSize() though;
		glm::vec2 mScenePanelOffset = { 0, 0 }; // Only in edit mode, can always call GetSceneWindowOffset() though;

		bool mRunning = true;
		bool mMinimized = false;
		static Application* mInstance;

		float mLastFrameTime = 0;
	};
}
