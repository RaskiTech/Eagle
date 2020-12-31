// OpenGl impl for index and vertex buffers
#pragma once

#include "Eagle/Rendering/Buffer.h"

namespace Egl {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetLayout(const BufferLayout& layout) override { mLayout = layout; }
		const BufferLayout& GetLayout() const { return mLayout; }

	private:
		BufferLayout mLayout;
		uint32_t mRendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline uint32_t GetCount() const override { return mCount; }

	private:
		uint32_t mCount;
		uint32_t mRendererID;
	};

}