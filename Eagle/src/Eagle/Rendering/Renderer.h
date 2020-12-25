#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

namespace Egl {
	class Renderer {
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vArray, const std::shared_ptr<Shader>& shader);

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* mSceneData;
	};
}