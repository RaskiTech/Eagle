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
		void SetViewportSize(const glm::vec2& size) { mViewportSize = size; }

		static inline Application& Get() { return *mInstance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		// Keep this at top because it terminates glfw context
		Scope<Window> mWindow;

		LayerStack mLayerStack;

		ImGuiLayer* mImGuiLayer;
		EditorLayer* mEditorLayer;
		GameLayer* mGameLayer;
		glm::vec2 mViewportSize = { 0, 0 };

		bool mRunning = true;
		bool mMinimized = false;
		static Application* mInstance;

		float mLastFrameTime = 0;
	};
}
