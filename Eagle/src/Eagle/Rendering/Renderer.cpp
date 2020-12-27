#include <EaglePCH.h>
#include "Renderer.h"
#include "RenderCommand.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderAPI.h"
#include "Texture.h"

// Decide what renderer the application will use. Currenly supported renderers: OpenGL
#pragma region DecideRenderer

// How deciding the renderer works:
// This file will look what renderer it will use at compile time. It will compile only the scripts that are for that renderer.
// To add a new renderer, create new function declarions in a platform specific file and include it as an option in here.

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

	};

	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

	void Renderer::Init() {
		RenderCommand::Init();
	}
	void Renderer::Shutdown() {

	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera) {
		mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

	}
	void Renderer::EndScene() {

	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color) {

	}

	void Renderer::Submit(const Ref<VertexArray>& vArray, const Ref<Shader>& shader) {
		shader->Bind();
		shader->UploadUniformMat4("uViewProjection", mSceneData->ViewProjectionMatrix);

		vArray->Bind();
		RenderCommand::DrawIndexed(vArray);
	}
}