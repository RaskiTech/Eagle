#pragma once
#include "Core.h"
#include "Window.h"
#include "Eagle/Events/Event.h"
#include "Eagle/Events/ApplicationEvent.h"
#include "Layers/LayerStack.h"
#include "Eagle/ImGui/ImGuiLayer.h"

#include "Eagle/Core/Time.h"

// Keep as long as this is rendering
#include "Eagle/Rendering/Shader.h"
#include "Eagle/Rendering/Buffer.h"
#include "Eagle/Rendering/VertexArray.h"
#include "Eagle/Components/CameraController.h"
#include "Eagle/Rendering/Texture.h"

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
		bool OnWindowResize(WindowResizeEvent& e);

		LayerStack mLayerStack;
		Scope<Window> mWindow;
		ImGuiLayer* mImGuiLayer;
		bool mRunning = true;
		bool mMinimized = false;
		static Application* mInstance;

		float mLastFrameTime = 0;

	// FOR RENDERING. REMOVE WHEN REMOVING THE RENDERING TEMP CODE
	public:
		Ref<Shader> mShader;
		Ref<VertexArray> mVertexArray;

		Ref<Shader> mTextureShader;
		Ref<VertexArray> mTextureVertexArray;
		Ref<Texture> mTexture;

		CameraController mCameraController;
	};

	// Client will define
	Application* CreateApplication();
}
