#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "RenderAPI.h"

// Issues with layers:
// Event blocking
// Batch order (maybe can use openGL)

namespace Egl {
	struct RendererStats {
		uint32_t drawCallCount = 0;
		uint32_t quadCount = 0;

		inline uint32_t GetDrawCallCount() { return drawCallCount; }
		inline uint32_t GetQuadCount() { return quadCount; }
		inline uint32_t GetVertexCount() { return quadCount * 4; }
		inline uint32_t GetIndexCount() { return quadCount * 6; }
		void ResetStats() {
			drawCallCount = 0;
			quadCount = 0;
		}
	};
	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float tilingFactor;
		float textureID;
	};
	struct QuadData {
		uint16_t depth;
		uint32_t placeIndex;

		// This is here because then the pointer doesn't need to jump when accessing this array
		QuadVertex quadVertices[4];
	};

	class Renderer {
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static inline RenderAPI::API GetAPI() { return RenderAPI::GetRendererAPI(); }

		static RendererStats& GetStats();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		static void Flush();

#pragma region DrawQuad Overloads
		static        void DrawColorQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static inline void DrawColorQuad(uint16_t depth, const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) { DrawColorQuad(depth, glm::vec3(position, 0), size, color); }

		static        void DrawRotatedColorQuad(uint16_t depth, const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static inline void DrawRotatedColorQuad(uint16_t depth, const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color) { DrawRotatedColorQuad(depth, glm::vec3(position, 0), rotation, size, color); }

		static        void DrawTextureQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color);
		static inline void DrawTextureQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)                                             { DrawTextureQuad(depth, position,     size, texture, 1, {1, 1, 1, 1}); }
		static inline void DrawTextureQuad(uint16_t depth, const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) { DrawTextureQuad(depth, glm::vec3(position, 0), size, texture, tilingFactor, color); }
		static inline void DrawTextureQuad(uint16_t depth, const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture)                                             { DrawTextureQuad(depth, glm::vec3(position, 0), size, texture, 1, {1, 1, 1, 1}); }

		static        void DrawRotatedTextureQuad(uint16_t depth, const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color);
		static inline void DrawRotatedTextureQuad(uint16_t depth, const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture)                                             { DrawRotatedTextureQuad(depth, position,               rotation, size, texture, 1, { 1, 1, 1, 1 }); }
		static inline void DrawRotatedTextureQuad(uint16_t depth, const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) { DrawRotatedTextureQuad(depth, glm::vec3(position, 0), rotation, size, texture, tilingFactor, color); }
		static inline void DrawRotatedTextureQuad(uint16_t depth, const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture)                                             { DrawRotatedTextureQuad(depth, glm::vec3(position, 0), rotation, size, texture, 1, { 1, 1, 1, 1 }); }

		static        void DrawTextureQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& color);
		static inline void DrawTextureQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture>& subTexture)                                             { DrawTextureQuad(depth, position,     size, subTexture, 1, {1, 1, 1, 1}); }
		static inline void DrawTextureQuad(uint16_t depth, const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& color) { DrawTextureQuad(depth, glm::vec3(position, 0), size, subTexture, tilingFactor, color); }
		static inline void DrawTextureQuad(uint16_t depth, const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture>& subTexture)                                             { DrawTextureQuad(depth, glm::vec3(position, 0), size, subTexture, 1, {1, 1, 1, 1}); }

		static        void DrawRotatedTextureQuad(uint16_t depth, const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& color);
		static inline void DrawRotatedTextureQuad(uint16_t depth, const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture>& subTexture)                                             { DrawRotatedTextureQuad(depth, position,               rotation, size, subTexture, 1, { 1, 1, 1, 1 }); }
		static inline void DrawRotatedTextureQuad(uint16_t depth, const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& color) { DrawRotatedTextureQuad(depth, glm::vec3(position, 0), rotation, size, subTexture, tilingFactor, color); }
		static inline void DrawRotatedTextureQuad(uint16_t depth, const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture>& subTexture)                                             { DrawRotatedTextureQuad(depth, glm::vec3(position, 0), rotation, size, subTexture, 1, { 1, 1, 1, 1 }); }
		
		static inline void DrawTextureQuad(uint16_t depth, const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) { glm::vec2 c[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} }; DrawTextureQuad(depth, transform, texture, c, tilingFactor, color); }
		static void DrawTextureQuad(uint16_t depth, const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec2 texCoords[4], float tilingFactor, const glm::vec4& color);
		static void DrawColorQuad(uint16_t depth, const glm::mat4& transform, const glm::vec4& color);
#pragma endregion

	private:
		static void StartNewBatch();
	};
}