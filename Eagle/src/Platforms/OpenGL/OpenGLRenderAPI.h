#pragma once
#include "Eagle/Rendering/RenderAPI.h"
#include <glad/glad.h>


namespace Egl {
	void RenderAPI::Init() {
		LOG_GL_STATUS("RenderAPI init ");
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}
	void RenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		LOG_GL_STATUS("RenderAPI set viewport ({1}, {2}, {3}, {4})", (unsigned int)x, (unsigned int)y, (unsigned int)width, (unsigned int)height);
		glViewport(x, y, width, height);
	}

	void RenderAPI::SetColor(const glm::vec4& color) {
		LOG_GL_STATUS("RenderAPI set color ({0}, {1}, {2}, {3})", color.r, color.g, color.b, color.a);
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void RenderAPI::Clear() {
		LOG_GL_STATUS("RenderAPI clear");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void RenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		LOG_GL_STATUS("RenderAPI drawIndexed");
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
