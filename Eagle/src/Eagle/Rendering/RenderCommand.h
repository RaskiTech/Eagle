#pragma once
#include "VertexArray.h"
#include "RenderAPI.h"



namespace Egl {
	class RenderCommand {
	public:
		static inline void SetColor(const glm::vec4& color) { sRenderer->SetColor(color); }
		static inline void Clear() { sRenderer->Clear(); }

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vArray) {
			sRenderer->DrawIndexed(vArray);
		}

	private:
		static RenderAPI* sRenderer;
	};
}