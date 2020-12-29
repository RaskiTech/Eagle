#include <EaglePCH.h>
#include "Renderer.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>


// Decide what renderer the application will use. Currenly supported renderers: OpenGL
#pragma region DecideRenderer

// How deciding the renderer works:
// This file will look what renderer it will use at compile time. It will compile only the scripts that are for that renderer.
// To add a new renderer, create new function declarions in a platform specific file and include it as an option in here.

//#define LOG_GL_STATUS_ENABLE
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderAPI.h"
#include "Texture.h"

#ifdef LOG_GL_STATUS_ENABLE
	#define LOG_GL_STATUS(...) LOG_ENG(__VA_ARGS__)
#else
	#define LOG_GL_STATUS(...)
#endif

#ifdef EAGLE_RENDERER_OPENGL
	#include "Platforms/OpenGL/OpenGLShader.h"
	#include "Platforms/OpenGL/OpenGLBuffer.h"
	#include "Platforms/OpenGL/OpenGLVertexArray.h"
	#include "Platforms/OpenGL/OpenGLRenderAPI.h"
	#include "Platforms/OpenGL/OpenGLTexture.h"
#else
	#error Eagle currently only supports OpenGL
#endif

#pragma endregion

namespace Egl {

	struct RendererStorage {
		Ref<VertexArray> vertexArray;
		Ref<Shader> colorShader;
		Ref<Shader> textureShader;
	};
	static RendererStorage* sRendererData;

	void Renderer::Init() {
		sRendererData = new RendererStorage();

		RenderCommand::Init();
		sRendererData->vertexArray = VertexArray::Create();

		// Keep these at 1s and -1s
		float vertices[] = {
			-1, -1, 0, 0,
			 1, -1, 1, 0,
			 1,  1, 1, 1,
			-1,  1, 0, 1
		};

		Ref<VertexBuffer> mVertexBuffer;
		mVertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		mVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "Position" },
			{ ShaderDataType::Float2, "TextureCoordinates" }
		});
		sRendererData->vertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[] = {
			0, 1, 2, 0, 3, 2
		};

		Ref<IndexBuffer> mIndexBuffer;
		mIndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		sRendererData->vertexArray->SetIndexBuffer(mIndexBuffer);

		std::string vertexColorSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			void main() {
				gl_Position = uViewProjection * uTransform * vec4(position, 0.0, 1.0);
			}
		)";
		std::string fragmentColorSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			
			uniform vec4 uColor;

			void main() { 
				color = uColor;
			}
		)";

		std::string vertexTextureSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec2 aTexCoord;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;
			out vec2 vTexCoord;

			void main() {
				vTexCoord = aTexCoord;
				gl_Position = uViewProjection * uTransform * vec4(position, 0.0, 1.0);
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

		sRendererData->colorShader = Shader::Create(vertexColorSource, fragmentColorSource);
		sRendererData->textureShader = Shader::Create(vertexTextureSource, fragmentColorSource);
		sRendererData->textureShader->Bind();
		sRendererData->textureShader->SetInt("uTexture", 0);
	}
	void Renderer::Shutdown() {
		delete sRendererData;
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera) {
		sRendererData->colorShader->Bind();
		sRendererData->colorShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());

		sRendererData->textureShader->Bind();
		sRendererData->textureShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color) {
		sRendererData->colorShader->Bind();
		sRendererData->colorShader->SetFloat4("uColor", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0 });
		sRendererData->colorShader->SetMat4("uTransform", transform);

		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
	void Renderer::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture) {
		sRendererData->textureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0 });
		sRendererData->textureShader->SetMat4("uTransform", transform);

		texture->Bind(0);

		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}

	void Renderer::EndScene() {

	}
}