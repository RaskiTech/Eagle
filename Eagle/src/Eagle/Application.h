#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Layers/LayerStack.h"
#include "Eagle/ImGui/ImGuiLayer.h"

#include "Eagle/Core/Time.h"

// Keep as long as this is rendering
#include "Rendering/Shader.h"
#include "Rendering/Buffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Camera.h"

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

		float mLastFrameTime = 0;

	// FOR RENDERING. REMOVE WHEN REMOVING THE RENDERING TEMP CODE
	public:
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<VertexArray> mVertexArray;

		Camera mCamera;
	};

	// Client will define
	Application* CreateApplication();
}
