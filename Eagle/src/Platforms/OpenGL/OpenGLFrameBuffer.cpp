#include <EaglePCH.h>
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Egl {
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferDefenition& defenition) : mDefenition(defenition)
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(1, &mColorAttachment);
		glDeleteTextures(1, &mDepthAttatchment);
	}

	void OpenGLFrameBuffer::Invalidate() {
		// Delete the old one if it exists
		if (mRendererID != 0) {
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(1, &mColorAttachment);
			glDeleteTextures(1, &mDepthAttatchment);
		}

		glCreateFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
		glBindTexture(GL_TEXTURE_2D, mColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, mDefenition.width, mDefenition.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttatchment);
		glBindTexture(GL_TEXTURE_2D, mDepthAttatchment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mDefenition.width, mDefenition.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttatchment, 0);


		EAGLE_ENG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer isn't complete!");

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0, 0, mDefenition.width, mDefenition.height);
	}
	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		EAGLE_ENG_ASSERT(width > 0 && height > 0, 
			"Since this triggered, add a check to run below code only when width and height are above 0");

		mDefenition.width = width;
		mDefenition.height = height;
		Invalidate();
	}
}