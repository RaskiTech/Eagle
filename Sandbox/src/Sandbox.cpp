#include <Eagle.h>

#include <glm/glm.hpp>
#include "ImGui/imgui.h"


#include "Eagle/Rendering/VertexArray.h"
#include "Eagle/Rendering/Shader.h"
#include "Eagle/Rendering/Buffer.h"
using namespace Egl;

class BaseLayer : public Layer {
public:
	BaseLayer() : Layer("BaseLayer", true) 
	{
		/*
		testPointer.reset(new VertexArray());
		
		float vertices[6] = {
			-0.8f, -0.8f,
			 0.8f, -0.8f,
			 0.0f,    0.6f
		};

		VertexBuffer mVertexBuffer(vertices, sizeof(vertices));
		//*
		
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

			uniform mat4 uViewProjection;

			void main() {
				gl_Position = uViewProjection * vec4(position, 0.0, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;

			void main() { 
				color = vec4(0.2f, 0.8f, 0.1f, 1.0f);
			}
		)";

		mShader.reset(new Shader(vertexSource, fragmentSource));
		*/
	}

	void OnUpdate() override {
		LOG(Time::GetDeltaTime());
		if (Input::IsKeyPressed(EGL_KEY_A)) Application::Get().mCamera.SetPosition(Application::Get().mCamera.GetPosition() + glm::vec3(-0.1, 0, 0) * Time::GetDeltaTime());
		if (Input::IsKeyPressed(EGL_KEY_D)) Application::Get().mCamera.SetPosition(Application::Get().mCamera.GetPosition() - glm::vec3(-0.1, 0, 0) * Time::GetDeltaTime());

		RenderCommand::SetColor({ 0.1f, 0.1f, 0.2f, 1 });
		RenderCommand::Clear();

		Renderer::BeginScene(Application::Get().mCamera);
		Renderer::Submit(Application::Get().mVertexArray, Application::Get().mShader);
		//Renderer::DrawQuad({ 0, 0 }, { 1, 1 }, { 1, 0, 0, 1 });
		Renderer::EndScene();
	}

	void OnEvent(Egl::Event& event) override {
		//if (event.GetEventType() == Egl::EventType::KeyPressed)
		//	LOG("Key pressed: {0}", event.GetName());
		//LOG_INFO("Event occured: {0}", event);
	}

	/*virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Test");
		ImGui::End();
	}*/
};

class Sandbox : public Egl::Application {
public:
	Sandbox() {
		AddLayer(new BaseLayer());
	}

	~Sandbox() {

	}
};

Egl::Application* Egl::CreateApplication() {
	return new Sandbox();
}