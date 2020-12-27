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
		{
			float vertices[6] = {
				-0.8f, -0.8f,
				 0.8f, -0.8f,
				 0.0f,    0.6f
			};
			Ref<VertexBuffer> buffer;
			buffer.reset(new VertexBuffer(vertices, sizeof(vertices)));
		}
		/*
		Application::Get().mVertexArray.reset(new VertexArray());
		
		float vertices[6] = {
			-0.8f, -0.8f,
			 0.8f, -0.8f,
			 0.0f,    0.6f
		};

		Ref<VertexBuffer> mVertexBuffer;
		mVertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));
		mVertexBuffer->SetLayout({
			{ShaderDataType::Float2, "position"},
			{ShaderDataType::Float4, "color"}
		});
		Application::Get().mVertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[3] = {
			0, 1, 2
		};

		Ref<IndexBuffer> mIndexBuffer;
		mIndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		Application::Get().mVertexArray->SetIndexBuffer(mIndexBuffer);

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

		Application::Get().mShader.reset(new Shader(vertexSource, fragmentSource));
		//*/
	}

	void OnUpdate() override {
		Application::Get().mCameraController.OnUpdate();

		RenderCommand::SetColor({ 0.1f, 0.1f, 0.2f, 1 });
		RenderCommand::Clear();

		Renderer::BeginScene(Application::Get().mCameraController.GetCamera());
		Renderer::Submit(Application::Get().mVertexArray, Application::Get().mShader);
		Application::Get().mTexture->Bind();
		Renderer::Submit(Application::Get().mTextureVertexArray, Application::Get().mTextureShader);

		//Renderer::DrawQuad({0, 0}, {1, 1}, {1, 0, 0, 1});
		Renderer::EndScene();
	}

	void OnEvent(Egl::Event& event) override {
		Application::Get().mCameraController.OnEvent(event);
	}
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