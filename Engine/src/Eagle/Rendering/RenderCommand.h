#pragma once
#include "VertexArray.h"
#include "RenderAPI.h"



namespace Egl {
	class RenderCommand {
	public:
		static inline void Init() { sRenderer->Init(); }

		static inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { sRenderer->SetViewport(x, y, width, height); }
		static inline void SetColor(const glm::vec4& color) { sRenderer->SetColor(color); }
		static inline void Clear() { sRenderer->Clear(); }

		static inline void DrawIndexed(const Ref<VertexArray>& vArray, uint32_t indexCount = 0) { sRenderer->DrawIndexed(vArray, indexCount); }

	private:
		static RenderAPI* sRenderer;
	};
}