#include <EaglePCH.h>
#include <glad/glad.h>
#include "OpenGLVertexArray.h"

namespace Egl {

	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type) {
		switch (type) {
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

	OpenGLVertexArray::OpenGLVertexArray() {
		EAGLE_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &mRendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &mRendererID);
	}
	void OpenGLVertexArray::Bind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindVertexArray(mRendererID);
	}
	void OpenGLVertexArray::Unbind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer) {
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
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer) {
		EAGLE_PROFILE_FUNCTION();

		glBindVertexArray(mRendererID);
		buffer->Bind();
		mIndexBuffer = buffer;
	}
}