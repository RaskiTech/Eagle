#pragma once

// OpenGl impl for index and vertex buffers

#include <EaglePCH.h>
#include "Eagle/Rendering/Buffer.h"
#include <glad/glad.h>

namespace Egl {

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Vertex buffers ////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
		EAGLE_PROFILE_FUNCTION();

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	VertexBuffer::~VertexBuffer() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &mRendererID);
	}
	void VertexBuffer::Bind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}
	void VertexBuffer::Unbind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Index buffers /////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : mCount(count) {
		EAGLE_PROFILE_FUNCTION();
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	IndexBuffer::~IndexBuffer() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &mRendererID);
	}
	void IndexBuffer::Bind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}
	void IndexBuffer::Unbind() const {
		EAGLE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}