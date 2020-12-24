#include "EaglePCH.h"
#include "Application.h"
#include <glad/glad.h>
#include "Input.h"

namespace Egl {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type) {
		switch (type)
		{
			case Egl::ShaderDataType::Float:   return GL_FLOAT;
			case Egl::ShaderDataType::Float2:  return GL_FLOAT;
			case Egl::ShaderDataType::Float3:  return GL_FLOAT;
			case Egl::ShaderDataType::Float4:  return GL_FLOAT;
			case Egl::ShaderDataType::Matrix3: return GL_FLOAT;
			case Egl::ShaderDataType::Matrix4: return GL_FLOAT;
			case Egl::ShaderDataType::Int:     return GL_INT;
			case Egl::ShaderDataType::Int2:    return GL_INT;
			case Egl::ShaderDataType::Int3:    return GL_INT;
			case Egl::ShaderDataType::Int4:    return GL_INT;
			case Egl::ShaderDataType::Bool:    return GL_BOOL;
		}
		EAGLE_CORE_ASSERT(false, "The shaderDataType was None or unknown");
		return 0;
	}

	Application::Application() {
		mInstance = this;
		
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);

		// Little less temp code to render a triangle
		// Rendering
		glGenVertexArrays(1, &mVertexArray);
		glBindVertexArray(mVertexArray);

		float vertices[] = {
			-0.8, -0.8, 1, 0, 1, 1,
			 0.8, -0.8, 0, 1, 1, 1,
			 0,    0.8, 1, 1, 0, 1
		};

		mVertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float2, "position"},
				{ShaderDataType::Float4, "color"}
			};

			mVertexBuffer->SetLayout(layout);
		}

		const auto& layout = mVertexBuffer->GetLayout();
		for (int i = 0; i < layout.GetLayout().size(); i++) {
			const auto& element = layout.GetLayout()[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.GetComponentCount(), ShaderDataTypeToOpenGL(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.offset);
		}

		uint32_t indices[3] = {
			0, 1, 2
		};
		mIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec4 color;

			out vec4 vColor;

			void main() {
				gl_Position = vec4(position, 0.0, 1.0);
				vColor = color;
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
			glClearColor(0.1f, 0.1f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			mShader->Bind();
			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, mIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : mLayerStack)
				if (layer->IsActive())
					layer->OnUpdate();

			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				if (layer->IsActive())
					layer->OnImGuiRender();
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		mRunning = false;
		return true;
	}
}