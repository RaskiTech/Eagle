#pragma once

#include <EaglePCH.h>
#include "Eagle/Rendering/VertexArray.h"
#include <glad/glad.h>

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
		EAGLE_ENG_ASSERT(false, "The shaderDataType was None or unknown");
		return 0;
	}


	VertexArray::VertexArray() {
		EAGLE_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &mRendererID);
	}
	VertexArray::~VertexArray() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &mRendererID);
	}

	void VertexArray::Bind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindVertexArray(mRendererID);
	}
	void VertexArray::Unbind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer) {
		EAGLE_PROFILE_FUNCTION();
		EAGLE_ENG_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex buffer layout doesn't exist");

		glBindVertexArray(mRendererID);
		buffer->Bind();

		
		const auto& layout = buffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(mVertexBufferInsertIndex);
			glVertexAttribPointer(mVertexBufferInsertIndex, element.GetComponentCount(), ShaderDataTypeToOpenGL(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(intptr_t)element.offset);
			mVertexBufferInsertIndex++;
		}
		mVertexBuffers.push_back(buffer);
	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
	{
		EAGLE_PROFILE_FUNCTION();

		glBindVertexArray(mRendererID);
		buffer->Bind();
		mIndexBuffer = buffer;
	}

	std::string VertexArray::ToString() {
		std::stringstream str;
		str << "VertexArray: (indexBuffer: count:" << GetIndexBuffer()->GetCount() << ") (vertexBuffers: ";
		for (Ref<VertexBuffer> buffer : GetVertexBuffers())
			str << " (elements count: " << buffer->GetLayout().GetElements().size() << ")";

		return str.str();
	}
}