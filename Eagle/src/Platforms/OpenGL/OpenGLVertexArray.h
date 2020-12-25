#pragma once

#include <EaglePCH.h>
#include "Eagle/Rendering/VertexArray.h"

namespace Egl {

	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type) {
		switch (type)
		{
			case Egl::ShaderDataType::Float:   return GL_FLOAT;
			case Egl::ShaderDataType::Float2:  return GL_FLOAT;
			case Egl::ShaderDataType::Float3:  return GL_FLOAT;
			case Egl::ShaderDataType::Float4:  return GL_FLOAT;
			case Egl::ShaderDataType::Matrix3: return GL_FLOAT;
			case Egl::ShaderDataType::Matrix4: return GL_FLOAT;
			case Egl::ShaderDataType::Int:     return GL_INT;
			case Egl::ShaderDataType::Int2:    return GL_INT;
			case Egl::ShaderDataType::Int3:    return GL_INT;
			case Egl::ShaderDataType::Int4:    return GL_INT;
			case Egl::ShaderDataType::Bool:    return GL_BOOL;
		}
		EAGLE_CORE_ASSERT(false, "The shaderDataType was None or unknown");
		return 0;
	}


	VertexArray::VertexArray() {
		glCreateVertexArrays(1, &mRendererID);
	}
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &mRendererID);
	}

	void VertexArray::Bind() const {
		glBindVertexArray(mRendererID);
	}
	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) {
		EAGLE_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex buffer layout doesn't exist");

		glBindVertexArray(mRendererID);
		buffer->Bind();

		const auto& layout = buffer->GetLayout();
		for (int i = 0; i < layout.GetElements().size(); i++) {
			const auto& element = layout.GetElements()[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.GetComponentCount(), ShaderDataTypeToOpenGL(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.offset);
		}
		mVertexBuffers.push_back(buffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
	{
		glBindVertexArray(mRendererID);
		buffer->Bind();
		mIndexBuffer = buffer;
	}
}