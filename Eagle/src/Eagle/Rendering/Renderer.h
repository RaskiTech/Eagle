#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "RenderAPI.h"

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

	class Renderer {
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static inline RenderAPI::API GetAPI() { return RenderAPI::GetRendererAPI(); }

		static RendererStats& GetStats();

		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Flush();

		#pragma region DrawRotatedQuadOverloads
		static void DrawRotatedColorQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static inline void DrawRotatedColorQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)                                                  { DrawRotatedColorQuad(glm::vec3(position, 0), rotation, size, color); }

		static void DrawRotatedTextureQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color);
		static inline void DrawRotatedTextureQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) { DrawRotatedTextureQuad(glm::vec3(position, 0), rotation, size, texture, tilingFactor, color); }
		
		static inline void DrawRotatedTextureQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture,                     const glm::vec4& color) { DrawRotatedTextureQuad(position, rotation, size, texture, 1,            color       ); }
		static inline void DrawRotatedTextureQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor                        ) { DrawRotatedTextureQuad(position, rotation, size, texture, tilingFactor, glm::vec4(1)); }
		static inline void DrawRotatedTextureQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture                                            ) { DrawRotatedTextureQuad(position, rotation, size, texture, 1,            glm::vec4(1)); }
		static inline void DrawRotatedTextureQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture,                     const glm::vec4& color) { DrawRotatedTextureQuad(position, rotation, size, texture, 1,            color       ); }
		static inline void DrawRotatedTextureQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor                        ) { DrawRotatedTextureQuad(position, rotation, size, texture, tilingFactor, glm::vec4(1)); }
		static inline void DrawRotatedTextureQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture                                            ) { DrawRotatedTextureQuad(position, rotation, size, texture, 1,            glm::vec4(1)); }
		#pragma endregion
		#pragma region DrawQuadOverloads
		static void DrawColorQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static inline void DrawColorQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)                                                    { DrawColorQuad(glm::vec3(position, 0), size, color); }

		static void DrawTextureQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color);
		static inline void DrawTextureQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) { DrawTextureQuad(glm::vec3(position, 0), size, texture, tilingFactor, color); }

		static inline void DrawTextureQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture,                     const glm::vec4& color) { DrawTextureQuad(position, size, texture, 1,            color       ); }
		static inline void DrawTextureQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor                        ) { DrawTextureQuad(position, size, texture, tilingFactor, glm::vec4(1)); }
		static inline void DrawTextureQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture                                            ) { DrawTextureQuad(position, size, texture, 1,            glm::vec4(1)); }
		static inline void DrawTextureQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture,                     const glm::vec4& color) { DrawTextureQuad(position, size, texture, 1,            color       ); }
		static inline void DrawTextureQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor                        ) { DrawTextureQuad(position, size, texture, tilingFactor, glm::vec4(1)); }
		static inline void DrawTextureQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture                                            ) { DrawTextureQuad(position, size, texture, 1,            glm::vec4(1)); }
		#pragma endregion
	private:
		static void SetQuadVertexBufferDataAndIncrement(QuadVertex*& ptr, const glm::vec3& position, 
			const glm::vec4& color, const glm::vec2& texCoord, float tiligFactor, float textureID);
		static void StartNewBatch();
	};
}