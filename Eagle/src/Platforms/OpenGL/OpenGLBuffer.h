#pragma once

// DO NOT INCLUDE ANYWHERE EXEPT IN Buffer.h
// OpenGl impl for index and vertex buffers. DO NOT INCLUDE

#include <EaglePCH.h>
#include "Eagle/Rendering/Buffer.h"
#include <glad/glad.h>

namespace Egl {

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Vertex buffers ////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &mRendererID);
	}
	void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}
	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//void VertexBuffer::SetLayout(const BufferLayout& layout) {mLayout = layout;}
	//const BufferLayout& VertexBuffer::GetLayout() const {return mLayout;}

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Index buffers /////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : mCount(count) {
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &mRendererID);
	}
	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}
	void IndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}