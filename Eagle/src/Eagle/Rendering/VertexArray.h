#pragma once
#include <EaglePCH.h>
#include "Eagle/Rendering/Buffer.h"

namespace Egl {
	class VertexArray {
	public:
		inline static VertexArray* Create() { return new VertexArray(); }
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer);


#ifdef EAGLE_RENDERER_OPENGL
	public:
		const auto& GetVertexBuffers() { return mVertexBuffers; }
		const auto& GetIndexBuffer() { return mIndexBuffer; } 
	private:

		uint32_t mRendererID;
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
#endif

	};
}