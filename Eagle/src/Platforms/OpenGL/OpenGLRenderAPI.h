#pragma once
#include "Eagle/Rendering/RenderAPI.h"
#include <glad/glad.h>


namespace Egl {
	void RenderAPI::SetColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void RenderAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void RenderAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
