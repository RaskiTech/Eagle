#include "EaglePCH.h"
#include "Application.h"
#include "Eagle/Rendering/Renderer.h"
#include "Eagle/Rendering/RenderCommand.h"

#include "GLFW/glfw3.h"

namespace Egl {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	Application::Application()
		: mCameraController(1280.0f / 720.0f, true)
	{
		mInstance = this;

		mWindow = Scope<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);

		// Little less temp code to render a triangle
		// Rendering
		
		mVertexArray.reset(new VertexArray());

		float vertices[] = {
			-0.8f + 1, -0.8f + 0.3f, 1, 0, 1, 1,
			 0.8f + 1, -0.8f + 0.3f, 0, 1, 1, 1,
			 0.0f + 1,  0.6f + 0.3f, 1, 1, 0, 1
		};

		Ref<VertexBuffer> mVertexBuffer;
		mVertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));
		mVertexBuffer->SetLayout({
			{ShaderDataType::Float2, "position"},
			{ShaderDataType::Float4, "color"}
		});
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[] = {
			0, 1, 2
		};

		Ref<IndexBuffer> mIndexBuffer;
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


		// Texture
		mTextureVertexArray.reset(new VertexArray());

		float textureVertices[] = {
			-0.8f + 1, -0.8f, 0.0f, 0.0f,
			 0.8f + 1,  0.8f, 1.0f, 1.0f,
			-0.8f + 1,  0.8f, 0.0f, 1.0f,
			 0.8f + 1, -0.8f, 1.0f, 0.0f
		};

		Ref<VertexBuffer> mTextureVertexBuffer;
		mTextureVertexBuffer.reset(new VertexBuffer(textureVertices, sizeof(textureVertices)));
		mTextureVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "Position" },
			{ ShaderDataType::Float2, "TextureCoordinates" }
		});
		mTextureVertexArray->AddVertexBuffer(mTextureVertexBuffer);

		uint32_t textureIndices[] = {
			0, 1, 2, 0, 3, 1
		};

		Ref<IndexBuffer> mTextureIndexBuffer;
		mTextureIndexBuffer.reset(new IndexBuffer(textureIndices, sizeof(textureIndices) / sizeof(uint32_t)));
		mTextureVertexArray->SetIndexBuffer(mTextureIndexBuffer);

		std::string vertexTextureSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec2 texCoord;

			out vec2 vTexCoord;

			uniform mat4 uViewProjection;

			void main() {
				vTexCoord = texCoord;
				gl_Position = uViewProjection * vec4(position, 0.0, 1.0);
			}
		)";

		std::string fragmentTextureSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec2 vTexCoord;

			uniform sampler2D uTexture;

			void main() { 
				color = texture(uTexture, vTexCoord);
			}
		)";

		mTextureShader.reset(new Shader(vertexTextureSource, fragmentTextureSource));
		mTexture = Texture::Create("Assets/EagleLogo.png");
		mTextureShader->Bind();
		mTextureShader->UploadUniformInt("uTexture", 0);
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {
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
		mLayerStack.AddLayer(layer);
		layer->OnAttach();
	}
	void Application::AddOverlay(Layer* layer) {
		mLayerStack.AddOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run() {
		while (mRunning) {
			// Time class
			float time = (float)glfwGetTime();
			Time::SetTime(time, time - mLastFrameTime);
			mLastFrameTime = time;

			if (!mMinimized) {
				// Update the layers
				for (Layer* layer : mLayerStack)
					if (layer->IsActive())
						layer->OnUpdate();
			}

			// ImGui update
			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				if (layer->IsActive())
					layer->OnImGuiRender();
			mImGuiLayer->End();
			// Update the window
			mWindow->OnUpdate();
		}

		Renderer::Shutdown();
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		mRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e) {
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