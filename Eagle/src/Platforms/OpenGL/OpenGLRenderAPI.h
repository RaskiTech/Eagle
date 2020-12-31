#pragma once
#include "Eagle/Rendering/RenderAPI.h"
#include <glad/glad.h>

namespace Egl {

	class OpenGLRenderAPI : public RenderAPI {
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};


}
