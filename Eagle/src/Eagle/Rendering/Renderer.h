#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

namespace Egl {
	class Renderer {
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vArray, const Ref<Shader>& shader);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color);
		static inline void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color) 
			{ DrawQuad(glm::vec3(position, 0.0f), size, color); }

		//*
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* mSceneData;
	};
}