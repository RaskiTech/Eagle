#pragma once
#include "Core.h"
#include "Window.h"
#include "Eagle/Events/Event.h"
#include "Eagle/Events/ApplicationEvent.h"
#include "Layers/LayerStack.h"
#include "Eagle/ImGui/ImGuiLayer.h"

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
		inline Window& GetWindow() const { return *mWindow; }
		static inline Application& Get() { return *mInstance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		LayerStack mLayerStack;
		Scope<Window> mWindow;
		ImGuiLayer* mImGuiLayer;
		bool mRunning = true;
		bool mMinimized = false;
		static Application* mInstance;

		float mLastFrameTime = 0;
	};

	// Client will define
	Application* CreateApplication();
}
