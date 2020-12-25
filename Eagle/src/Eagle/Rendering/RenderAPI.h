#pragma once
#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Egl {
	class RenderAPI {
	public:
		void SetColor(const glm::vec4& color);
		void Clear();

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	};
}