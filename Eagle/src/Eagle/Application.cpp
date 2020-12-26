#include "EaglePCH.h"
#include "Application.h"
#include "Input.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderCommand.h"

#include "Keycodes.h"

#include "GLFW/glfw3.h"

namespace Egl {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	Application::Application()
		: mCamera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		mInstance = this;

		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);

		// Little less temp code to render a triangle
		// Rendering

		mVertexArray.reset(new VertexArray());

		float vertices[] = {
			-0.8f, -0.8f, 1, 0, 1, 1,
			 0.8f, -0.8f, 0, 1, 1, 1,
			 0.0f,  0.6f, 1, 1, 0, 1
		};

		std::shared_ptr<VertexBuffer> mVertexBuffer;
		mVertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));
		mVertexBuffer->SetLayout({
			{ShaderDataType::Float2, "position"},
			{ShaderDataType::Float4, "color"}
		});
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[3] = {
			0, 1, 2
		};

		std::shared_ptr<IndexBuffer> mIndexBuffer;
		mIndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->SetIndexBuffer(mIndexBuffer);

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec4 color;

			uniform mat4 uViewProjection;
			out vec4 vColor;

			void main() {
				vColor = color;
				gl_Position = uViewProjection * vec4(position, 0.0, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec4 vColor;

			void main() { 
				color = vColor;
			}
		)";

		mShader.reset(new Shader(vertexSource, fragmentSource));
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));

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
		mLayerStack.AddLayer(layer);
		layer->OnAttach();
	}
	void Application::AddOverlay(Layer* layer) {
		mLayerStack.AddOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run() {
		while (mRunning) {
			float time = (float)glfwGetTime();
			Time::SetTime(time, time - mLastFrameTime);
			mLastFrameTime = time;

			// Update the layers
			for (Layer* layer : mLayerStack)
				if (layer->IsActive())
					layer->OnUpdate();

			// ImGui update
			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				if (layer->IsActive())
					layer->OnImGuiRender();
			mImGuiLayer->End();

			// Update the window
			mWindow->OnUpdate();
		}
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		mRunning = false;
		return true;
	}
}