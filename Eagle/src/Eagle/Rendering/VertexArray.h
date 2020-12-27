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

		void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
		void SetIndexBuffer(const  Ref<IndexBuffer>& buffer);


#ifdef EAGLE_RENDERER_OPENGL
	public:
		const auto& GetVertexBuffers() { return mVertexBuffers; }
		const auto& GetIndexBuffer() { return mIndexBuffer; } 
	private:

		uint32_t mRendererID;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
		Ref<IndexBuffer> mIndexBuffer;
#endif

	};
}