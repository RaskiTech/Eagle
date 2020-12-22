#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Layers/LayerStack.h"
#include "Eagle/ImGui/ImGuiLayer.h"
namespace Egl {

	class Application
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
		ImGuiLayer* mImGuiLayer;
		bool mRunning = true;
		static Application* mInstance;

		unsigned int mVertexArray, mVertexBuffer, mIndexBuffer;
	};

	// Client will define
	Application* CreateApplication();
}
