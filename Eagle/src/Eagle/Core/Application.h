#pragma once
#include "Core.h"
#include "Window.h"
#include "Eagle/Events/Event.h"
#include "Eagle/Events/ApplicationEvent.h"
#include "Layers/LayerStack.h"
#include "Eagle/ImGui/ImGuiLayer.h"
#include "Eagle/Scripting/GameLayer.h"

#include "Eagle/Core/Time.h"

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

		bool mRunning = true;
		bool mMinimized = false;
		static Application* mInstance;

		float mLastFrameTime = 0;
	};
}
