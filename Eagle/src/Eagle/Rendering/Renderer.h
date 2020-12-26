#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

namespace Egl {
	class Renderer {
	public:
		//static void Init();
		//static void Shutdown();

		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vArray, const std::shared_ptr<Shader>& shader);

		/*static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4 color);
		static inline void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4 color) 
			{ DrawQuad(glm::vec3(position, 0.0f), size, color); }*/

		
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* mSceneData;
		
	};
}