#include "EaglePCH.h"
#include "Application.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"

#include "GLFW/glfw3.h"

#include "Eagle/Rendering/VertexArray.h"
namespace Egl {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	Application::Application()
	{
		EAGLE_PROFILE_FUNCTION();

		mInstance = this;

		mWindow = Window::Create();
		mWindow->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);
#pragma region testingHeap

		Ref<VertexArray> array = VertexArray::Create();
		Ref<VertexArray> array0 = VertexArray::Create();
		Ref<VertexArray> array1 = VertexArray::Create();
		Ref<VertexArray> array2 = VertexArray::Create();
		Ref<VertexArray> array3 = VertexArray::Create();
		Ref<VertexArray> array4 = VertexArray::Create();
		Ref<VertexArray> array5 = VertexArray::Create();
		Ref<VertexArray> array6 = VertexArray::Create();
		Ref<VertexArray> array7 = VertexArray::Create();
		Ref<VertexArray> array8 = VertexArray::Create();
		Ref<VertexArray> array9 = VertexArray::Create();
		Ref<VertexArray> array12 = VertexArray::Create();
		Ref<VertexArray> array13 = VertexArray::Create();
		Ref<VertexArray> array14 = VertexArray::Create();
		Ref<VertexArray> array15 = VertexArray::Create();
		Ref<VertexArray> array16 = VertexArray::Create();
		Ref<VertexArray> array17 = VertexArray::Create();
		Ref<VertexArray> array18 = VertexArray::Create();
		Ref<VertexArray> array19 = VertexArray::Create();
		Ref<VertexArray> array20 = VertexArray::Create();
		Ref<VertexArray> array21 = VertexArray::Create();
		Ref<VertexArray> yarray = VertexArray::Create();
		Ref<VertexArray> yarray0 = VertexArray::Create();
		Ref<VertexArray> yarray1 = VertexArray::Create();
		Ref<VertexArray> yarray2 = VertexArray::Create();
		Ref<VertexArray> yarray3 = VertexArray::Create();
		Ref<VertexArray> yarray4 = VertexArray::Create();
		Ref<VertexArray> yarray5 = VertexArray::Create();
		Ref<VertexArray> yarray6 = VertexArray::Create();
		Ref<VertexArray> yarray7 = VertexArray::Create();
		Ref<VertexArray> yarray8 = VertexArray::Create();
		Ref<VertexArray> yarray9 = VertexArray::Create();
		Ref<VertexArray> yarray12 = VertexArray::Create();
		Ref<VertexArray> yarray13 = VertexArray::Create();
		Ref<VertexArray> yarray14 = VertexArray::Create();
		Ref<VertexArray> yarray15 = VertexArray::Create();
		Ref<VertexArray> yarray16 = VertexArray::Create();
		Ref<VertexArray> yarray17 = VertexArray::Create();
		Ref<VertexArray> yarray18 = VertexArray::Create();
		Ref<VertexArray> yarray19 = VertexArray::Create();
		Ref<VertexArray> yarray20 = VertexArray::Create();
		Ref<VertexArray> yarray21 = VertexArray::Create();

		auto texture2 =  Texture::Create("Assets/Water.png");
		auto texture3 =  Texture::Create("Assets/Water.png");
		auto texture4 =  Texture::Create("Assets/Water.png");
		auto texture5 =  Texture::Create("Assets/Water.png");
		auto texture6 =  Texture::Create("Assets/Water.png");
		auto texture7 =  Texture::Create("Assets/Water.png");
		auto texture8 =  Texture::Create("Assets/Water.png");
		auto texture9 =  Texture::Create("Assets/Water.png");
		auto texture0 =  Texture::Create("Assets/Water.png");
		auto texture1 =  Texture::Create("Assets/Water.png");
		auto texture  =  Texture::Create("Assets/Water.png");
		auto texture12 = Texture::Create("Assets/Water.png");
		auto texture13 = Texture::Create("Assets/Water.png");
		auto texture14 = Texture::Create("Assets/Water.png");
		auto texture15 = Texture::Create("Assets/Water.png");
		auto texture16 = Texture::Create("Assets/Water.png");
		auto texture17 = Texture::Create("Assets/Water.png");
		auto texture18 = Texture::Create("Assets/Water.png");
		auto texture19 = Texture::Create("Assets/Water.png");
		auto texture21 = Texture::Create("Assets/Water.png");
		auto texture22 = Texture::Create("Assets/Water.png");
		auto texture23 = Texture::Create("Assets/Water.png");
		auto texture24 = Texture::Create("Assets/Water.png");
		auto texture25 = Texture::Create("Assets/Water.png");
		
		auto qtexture2 = Texture::Create("Assets/Water.png");
		auto qtexture3 = Texture::Create("Assets/Water.png");
		auto qtexture4 = Texture::Create("Assets/Water.png");
		auto qtexture5 = Texture::Create("Assets/Water.png");
		auto qtexture6 = Texture::Create("Assets/Water.png");
		auto qtexture7 = Texture::Create("Assets/Water.png");
		auto qtexture8 = Texture::Create("Assets/Water.png");
		auto qtexture9 = Texture::Create("Assets/Water.png");
		auto qtexture0 = Texture::Create("Assets/Water.png");
		auto qtexture1 = Texture::Create("Assets/Water.png");
		auto qtexture = Texture::Create("Assets/Water.png");
		auto qtexture12 = Texture::Create("Assets/Water.png");
		auto qtexture13 = Texture::Create("Assets/Water.png");
		auto qtexture14 = Texture::Create("Assets/Water.png");
		auto qtexture15 = Texture::Create("Assets/Water.png");
		auto qtexture16 = Texture::Create("Assets/Water.png");
		auto qtexture17 = Texture::Create("Assets/Water.png");
		auto qtexture18 = Texture::Create("Assets/Water.png");
		auto qtexture19 = Texture::Create("Assets/Water.png");
		auto qtexture21 = Texture::Create("Assets/Water.png");
		auto qtexture22 = Texture::Create("Assets/Water.png");
		auto qtexture23 = Texture::Create("Assets/Water.png");
		auto qtexture24 = Texture::Create("Assets/Water.png");
		auto qtexture25 = Texture::Create("Assets/Water.png");
#pragma endregion
	}

	Application::~Application() {
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e) {
		EAGLE_PROFILE_SCOPE(e.GetName());
		EventDispatcher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
		dispacher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OnWindowResize));

		for (auto layer = mLayerStack.end(); layer != mLayerStack.begin();) {
			layer--;
			if ((*layer)->IsActive()) {
				(*layer)->OnEvent(e);
				if (e.IsHandled())
					break;
			}
		}
	}

	void Application::AddLayer(Layer* layer) {
		EAGLE_PROFILE_FUNCTION();
		mLayerStack.AddLayer(layer);
		layer->OnAttach();
	}
	void Application::AddOverlay(Layer* layer) {
		EAGLE_PROFILE_FUNCTION();
		mLayerStack.AddOverlay(layer);
		layer->OnAttach();
	}
	void Application::RemoveLayer(Layer* layer) {
		EAGLE_PROFILE_FUNCTION();
		layer->OnDetach();
		mLayerStack.RemoveLayer(layer);
	}
	void Application::RemoveOverlay(Layer* layer) {
		EAGLE_PROFILE_FUNCTION();
		layer->OnDetach();
		mLayerStack.RemoveOverlay(layer);
	}

	void Application::Run() {
		EAGLE_PROFILE_FUNCTION();
		while (mRunning) {
			EAGLE_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Time::SetTime(time, time - mLastFrameTime);
			mLastFrameTime = time;

			if (!mMinimized) {
				{
					EAGLE_PROFILE_SCOPE("Layer OnUpdates");
					// Update the layers
					for (Layer* layer : mLayerStack)
						if (layer->IsActive()) {
							EAGLE_PROFILE_SCOPE(layer->GetName().c_str());
							layer->OnUpdate();
						}
				}

				{
					EAGLE_PROFILE_SCOPE("ImGui update");
					mImGuiLayer->Begin();
					for (Layer* layer : mLayerStack)
						if (layer->IsActive())
							layer->OnImGuiRender();
					mImGuiLayer->End();
				}
			}

			mWindow->OnUpdate();
		}

		Renderer::Shutdown();
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		mRunning = false;
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
		return false;
	}
}