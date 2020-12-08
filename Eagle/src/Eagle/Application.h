#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Layers/LayerStack.h"

namespace Egl {

	class EAGLE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void AddLayer(Layer* layer);
		void AddOverlay(Layer* layer);
		static inline Application& Get() { return *mInstance; }
		inline Window& GetWindow() const { return *mWindow; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		LayerStack mLayerStack;
		std::unique_ptr<Window> mWindow;
		bool mRunning = true;
		static Application* mInstance;
	};

	// Client will define
	Application* CreateApplication();
}
