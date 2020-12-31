#pragma once

#include <vector>
#include "Eagle/Rendering/VertexArray.h"

namespace Egl {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return mVertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return mIndexBuffer; }
	private:
		uint32_t mRendererID;
		uint32_t mVertexBufferInsertIndex = 0;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
		Ref<IndexBuffer> mIndexBuffer;
	};

}