#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

namespace Egl {
	class Renderer {
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& color);
		#pragma region DrawQuadOverloads
		static inline void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture)                         { DrawQuad(position, rotation, size, texture, glm::vec4(1)); }
		// Pos 3 color
		static inline void DrawQuad(const glm::vec3& position, float rotation, const glm::vec4& color)							                           { DrawQuad(          position,     rotation, {1, 1}, color); }
		static inline void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)					                           { DrawQuad(          position,     0,        size,   color); }
		static inline void DrawQuad(const glm::vec3& position, const glm::vec4& color)											                           { DrawQuad(          position,     0,        {1, 1}, color); }
		// Pos 3 texture
		static inline void DrawQuad(const glm::vec3& position, float rotation, const Ref<Texture>& texture)				                                   { DrawQuad(          position,     rotation, {1, 1}, texture); }
		static inline void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)                                         { DrawQuad(          position,     0,        size,   texture); }
		static inline void DrawQuad(const glm::vec3& position, const Ref<Texture>& texture)	                                                               { DrawQuad(          position,     0,        {1, 1}, texture); }
		// Pos 3 texture color
		static inline void DrawQuad(const glm::vec3& position, float rotation, const Ref<Texture>& texture, const glm::vec4& color)	                       { DrawQuad(          position,     rotation, {1, 1}, texture, color); }
		static inline void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& color)                 { DrawQuad(          position,     0,        size,   texture, color); }
		static inline void DrawQuad(const glm::vec3& position, const Ref<Texture>& texture, const glm::vec4& color)	                                       { DrawQuad(          position,     0,        {1, 1}, texture, color); }
		// Pos 2 all
		static inline void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)                              { DrawQuad(glm::vec3(position, 0), rotation, size, color); }
		static inline void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture)                         { DrawQuad(glm::vec3(position, 0), rotation, size, texture); }
		static inline void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& color) { DrawQuad(glm::vec3(position, 0), rotation, size, texture, color); }
		// Pos 2 color
		static inline void DrawQuad(const glm::vec2& position, float rotation, const glm::vec4& color)                                                     { DrawQuad(glm::vec3(position, 0), rotation, color); }
		static inline void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)                                              { DrawQuad(glm::vec3(position, 0), size,   color); }
		static inline void DrawQuad(const glm::vec2& position, const glm::vec4& color)                                                                     { DrawQuad(glm::vec3(position, 0), color); }
		// Pos 2 texture
		static inline void DrawQuad(const glm::vec2& position, float rotation, const Ref<Texture>& texture)                                                { DrawQuad(glm::vec3(position, 0), rotation, texture); }
		static inline void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture)                                         { DrawQuad(glm::vec3(position, 0), size,   texture); }
		static inline void DrawQuad(const glm::vec2& position, const Ref<Texture>& texture)                                                                { DrawQuad(glm::vec3(position, 0), texture); }
		// Pos 2 texture color
		static inline void DrawQuad(const glm::vec2& position, float rotation, const Ref<Texture>& texture, const glm::vec4& color)                        { DrawQuad(glm::vec3(position, 0), rotation, texture, color); }
		static inline void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, const glm::vec4& color)                 { DrawQuad(glm::vec3(position, 0), size, texture, color); }
		static inline void DrawQuad(const glm::vec2& position, const Ref<Texture>& texture, const glm::vec4& color)                                        { DrawQuad(glm::vec3(position, 0), texture, color); }
		#pragma endregion

	};
}