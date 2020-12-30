#include <EaglePCH.h>
#include "Renderer.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

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
		Ref<Shader> quadShader;
		Ref<Texture> whiteTexture;
	};
	static RendererStorage* sRendererData;

	void Renderer::Init() {
		EAGLE_PROFILE_FUNCTION();
		//*
		sRendererData = new RendererStorage();

		RenderCommand::Init();
		sRendererData->vertexArray = VertexArray::Create();

		// Keep these at 1s and -1s
		float vertices[] = {
			-1, -1, 0, 0, 0,
			 1, -1, 0, 1, 0,
			 1,  1, 0, 1, 1,
			-1,  1, 0, 0, 1
		};

		Ref<VertexBuffer> mVertexBuffer;
		mVertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		mVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float2, "TextureCoordinates" }
		});
		sRendererData->vertexArray->AddVertexBuffer(mVertexBuffer);

		uint32_t indices[] = {
			0, 1, 2, 0, 3, 2
		};

		Ref<IndexBuffer> mIndexBuffer;
		mIndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		sRendererData->vertexArray->SetIndexBuffer(mIndexBuffer);
		sRendererData->whiteTexture = Texture::Create(1, 1);
		uint32_t data = 0xffffffff;
		sRendererData->whiteTexture->SetData(&data, sizeof(uint32_t));

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec2 aTexCoord;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;
			out vec2 vTexCoord;

			void main() {
				vTexCoord = aTexCoord;
				gl_Position = uViewProjection * uTransform * vec4(position, 1.0);
			}
		)";
		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			
			in vec2 vTexCoord;
			uniform sampler2D uTexture;
			uniform vec4 uColor;

			void main() { 
				color = texture(uTexture, vTexCoord * 10) * uColor;
			}
		)";

		sRendererData->quadShader = Shader::Create(vertexSource, fragmentSource);
		sRendererData->quadShader->Bind();
		sRendererData->quadShader->SetInt("uTexture", 0);
		// */
		/*
		EAGLE_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		unsigned int mVertexArrayID;
		glCreateVertexArrays(1, &mVertexArrayID);

		// Keep these at 1s and -1s
		float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		unsigned int mVertexBufferID;
		glCreateBuffers(1, &mVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glBindVertexArray(mVertexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBufferID);


		uint32_t indices[] = {
			0, 1, 2, 0, 3, 2
		};

		unsigned int mIndexBufferID;
		glCreateBuffers(1, &mIndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(mRendererID);
		buffer->Bind();
		sRendererData->vertexArray->SetIndexBuffer(mIndexBufferID);
		sRendererData->whiteTexture = Texture::Create(1, 1);
		uint32_t data = 0xffffffff;
		sRendererData->whiteTexture->SetData(&data, sizeof(uint32_t));

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec2 aTexCoord;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;
			out vec2 vTexCoord;

			void main() {
				vTexCoord = aTexCoord;
				gl_Position = uViewProjection * uTransform * vec4(position, 0.0, 1.0);
			}
		)";
		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			
			in vec2 vTexCoord;
			uniform sampler2D uTexture;
			uniform vec4 uColor;

			void main() { 
				color = texture(uTexture, vTexCoord * 10) * uColor;
			}
		)";

		sRendererData->quadShader = Shader::Create(vertexSource, fragmentSource);
		sRendererData->quadShader->Bind();
		sRendererData->quadShader->SetInt("uTexture", 0);
		*/
	}
	void Renderer::Shutdown() {
		EAGLE_PROFILE_FUNCTION();
		delete sRendererData;
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		EAGLE_PROFILE_FUNCTION();
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera) {
		EAGLE_PROFILE_FUNCTION();
		sRendererData->quadShader->Bind();
		sRendererData->quadShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		sRendererData->quadShader->SetFloat4("uColor", color);
		sRendererData->whiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0 });
		sRendererData->quadShader->SetMat4("uTransform", transform);
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
	void Renderer::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();
		sRendererData->quadShader->SetFloat4("uColor", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0 });
		sRendererData->quadShader->SetMat4("uTransform", transform);

		texture->Bind(0);

		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}

	void Renderer::EndScene() {
		EAGLE_PROFILE_FUNCTION();
	}
}