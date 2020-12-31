#pragma once
#include <memory>
#include "Eagle/Rendering/Buffer.h"

namespace Egl {
	class VertexArray {
	public:
		static Ref<VertexArray> Create();
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;
		virtual void SetIndexBuffer(const  Ref<IndexBuffer>& buffer) = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
	};
}