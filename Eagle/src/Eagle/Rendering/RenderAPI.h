#pragma once
#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Egl {
	class RenderAPI {
	public:
		enum class API {
			None, OpenGL
		};
		inline static API GetRendererAPI() { return sAPI; }

		static Scope<RenderAPI> Create();
		virtual ~RenderAPI() = default;
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;

	private:
		static API sAPI;
	};
}