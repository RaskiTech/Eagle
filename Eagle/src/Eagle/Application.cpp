#include "EaglePCH.h"
#include "Application.h"
#include "Input.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderCommand.h"

#include "Keycodes.h"

namespace Egl {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	Application::Application()
		: mCamera(-1.6, 1.6, -0.9, 0.9)
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
			-0.8, -0.8, 1, 0, 1, 1,
			 0.8, -0.8, 0, 1, 1, 1,
			 0,    0.6, 1, 1, 0, 1
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
		// The main loop
		while (mRunning) {
			Egl::RenderCommand::SetColor({ 0.1f, 0.1f, 0.2f, 1 });
			Egl::RenderCommand::Clear();

			//mCamera.SetPosition({ 0.5f, 0.5f, 0.5f });
			//mCamera.SetRotation(45);

			
			if (Egl::Input::IsKeyPressed(EGL_KEY_A)) mCamera.SetPosition(mCamera.GetPosition() + glm::vec3(-0.01, 0, 0));
			if (Egl::Input::IsKeyPressed(EGL_KEY_D))	mCamera.SetPosition(mCamera.GetPosition() + glm::vec3(0.01, 0, 0));
			if (Egl::Input::IsKeyPressed(EGL_KEY_W))	mCamera.SetPosition(mCamera.GetPosition() + glm::vec3(0, 0.01, 0));
			if (Egl::Input::IsKeyPressed(EGL_KEY_S))	mCamera.SetPosition(mCamera.GetPosition() + glm::vec3(0, -0.01, 0));

			if (Egl::Input::IsKeyPressed(EGL_KEY_Q))	mCamera.SetRotation(mCamera.GetRotation() + 1);
			if (Egl::Input::IsKeyPressed(EGL_KEY_E))	mCamera.SetRotation(mCamera.GetRotation() - 1);
			
			Egl::Renderer::BeginScene(mCamera);
			Egl::Renderer::Submit(mVertexArray, mShader);
			Egl::Renderer::EndScene();

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