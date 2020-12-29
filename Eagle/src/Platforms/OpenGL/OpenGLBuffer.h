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

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) : mLayout({}) {
		glCreateBuffers(1, &mRendererID);
		LOG_GL_STATUS("vertexBuffer init {0} {1}", (unsigned int)mRendererID, size);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	VertexBuffer::~VertexBuffer() {
		LOG_GL_STATUS("Vertexbuffer destroy {0}", (unsigned int)mRendererID);
		glDeleteBuffers(1, &mRendererID);
	}
	void VertexBuffer::Bind() const {
		LOG_GL_STATUS("Vertexbuffer bind {0}", (unsigned int)mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}
	void VertexBuffer::Unbind() const {
		LOG_GL_STATUS("Vertexbuffer unbind");
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//void VertexBuffer::SetLayout(const BufferLayout& layout) {mLayout = layout;}
	//const BufferLayout& VertexBuffer::GetLayout() const {return mLayout;}

	///////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Index buffers /////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : mCount(count) {
		glCreateBuffers(1, &mRendererID);
		LOG_GL_STATUS("Indexbuffer init {0}", (unsigned int)mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	IndexBuffer::~IndexBuffer() {
		LOG_GL_STATUS("Indexbuffer delete {0}", (unsigned int)mRendererID);
		glDeleteBuffers(1, &mRendererID);
	}
	void IndexBuffer::Bind() const {
		LOG_GL_STATUS("indexbuffer bind {0}", (unsigned int)mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}
	void IndexBuffer::Unbind() const {
		LOG_GL_STATUS("indexbuffer unbind");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}