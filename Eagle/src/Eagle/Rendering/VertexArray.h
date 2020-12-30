#pragma once
#include <memory>
#include "Eagle/Rendering/Buffer.h"

namespace Egl {
	class VertexArray {
	public:
		inline static Ref<VertexArray> Create() { return CreateRef<VertexArray>(); }
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
		void SetIndexBuffer(const  Ref<IndexBuffer>& buffer);
		std::string ToString();

		const auto& GetVertexBuffers() { return mVertexBuffers; }
		const auto& GetIndexBuffer() { return mIndexBuffer; } 
	private:
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
		Ref<IndexBuffer> mIndexBuffer;
		uint32_t mVertexBufferInsertIndex = 0;

#ifdef EAGLE_RENDERER_OPENGL
	private:
		uint32_t mRendererID;
#endif

	};
}