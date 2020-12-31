#include <EaglePCH.h>
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace Egl {

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Vertex buffers ////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		EAGLE_PROFILE_FUNCTION();

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &mRendererID);
	}
	void OpenGLVertexBuffer::Bind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}
	void OpenGLVertexBuffer::Unbind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Index buffers /////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : mCount(count) {
		EAGLE_PROFILE_FUNCTION();
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &mRendererID);
	}
	void OpenGLIndexBuffer::Bind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}
	void OpenGLIndexBuffer::Unbind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}