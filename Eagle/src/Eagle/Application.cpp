#include "EaglePCH.h"
#include "Application.h"
#include <glad/glad.h>
#include "Input.h"

namespace Egl {

#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::mInstance = nullptr;

	Application::Application() {
		mInstance = this;
		
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

		mImGuiLayer = new ImGuiLayer();
		AddOverlay(mImGuiLayer);

		// Temp code to render a triangle
		glGenVertexArrays(1, &mVertexArray);
		glBindVertexArray(mVertexArray);

		glGenBuffers(1, &mVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f,
			0.5f, -0.5,
			0, 0.5f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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