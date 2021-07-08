#include <EaglePCH.h>
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Egl {
	static const uint32_t maxFramebufferSize = 8192;

	static GLenum TextureTarget(bool hasMultipleSamples) {
		return hasMultipleSamples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}
	static void CreateTextures(bool hasMultipleSamples, uint32_t size, uint32_t* data) {
		glCreateTextures(TextureTarget(hasMultipleSamples), size, data);
	}
	static void BindTexture(bool hasMultipleSamples, uint32_t id) {
		glBindTexture(TextureTarget(hasMultipleSamples), id);
	}
	static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
		bool hasMultipleSamples = samples > 1;
		if (hasMultipleSamples)
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(hasMultipleSamples), id, 0);
	}
	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
		bool hasMultipleSamples = samples > 1;
		if (hasMultipleSamples)
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		else {
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(hasMultipleSamples), id, 0);
	}
	static GLenum FramebufferSpecFormatToGL(FramebufferTextureFormat format) {
		switch (format) {
		case FramebufferTextureFormat::RED_INT:         return GL_RED_INTEGER;
		case FramebufferTextureFormat::RGBA8:           return GL_RGBA8;
		case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
		}

		EAGLE_ENG_ASSERT(false, "FramebufferSpecFormatToGL doesn't have a return type for the used format");
		return 0;
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferDefenition& defenition) : mDefenition(defenition)
	{
		// The attachments can't change after the buffer has been made, so deal with them
		for (auto& format : defenition.attachments.attachments) {
			if (Framebuffer::IsDepthFormat(format.format))
				mDepthAttachmentSpecification = format.format;
			else
				mColorAttachmentSpecifications.emplace_back(format.format);
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate() {
		// Delete the old one if it exists
		if (mRendererID != 0) {
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
			glDeleteTextures(1, &mDepthAttachment);

			mColorAttachments.clear();
			mDepthAttachment = (uint32_t)FramebufferTextureFormat::None;
		}

		glCreateFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

		bool hasMultipleSamples = mDefenition.samples > 1;

		if (mColorAttachmentSpecifications.size() != 0) {
			mColorAttachments.resize(mColorAttachmentSpecifications.size());
			CreateTextures(hasMultipleSamples, mColorAttachments.size(), mColorAttachments.data());

			for (size_t i = 0; i < mColorAttachments.size(); i++) {
				BindTexture(hasMultipleSamples, mColorAttachments[i]);
				switch (mColorAttachmentSpecifications[i].format) {
				case FramebufferTextureFormat::RGBA8:
					AttachColorTexture(mColorAttachments[i], mDefenition.samples, GL_RGBA8, GL_RGBA, mDefenition.width, mDefenition.height, i);
					break;
				case FramebufferTextureFormat::RED_INT:
					AttachColorTexture(mColorAttachments[i], mDefenition.samples, GL_R32UI, GL_RED_INTEGER, mDefenition.width, mDefenition.height, i);
					break;
				}
			}
		}
		if (mDepthAttachmentSpecification.format != FramebufferTextureFormat::None) {
			CreateTextures(hasMultipleSamples, 1, &mDepthAttachment);
			BindTexture(hasMultipleSamples, mDepthAttachment);
			switch (mDepthAttachmentSpecification.format) {
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(mDepthAttachment, mDefenition.samples, GL_DEPTH24_STENCIL8,
					GL_DEPTH_STENCIL_ATTACHMENT, mDefenition.width, mDefenition.height);
			}
		}

		if (mColorAttachmentSpecifications.size() > 1) {
			EAGLE_ENG_ASSERT(mColorAttachmentSpecifications.size() < 5, "Currently only 4 color attachments are supported.");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(mColorAttachments.size(), buffers);
		}
		else if (mColorAttachments.empty()) {
			glDrawBuffer(GL_NONE);
		}

		EAGLE_ENG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer isn't complete!");

		// Unbind Framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0, 0, mDefenition.width, mDefenition.height);
	}
	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		EAGLE_ENG_ASSERT(width > 0 && height > 0,
			"Since this triggered, add a check to run below code only when width and height are above 0");

		mDefenition.width = width;
		mDefenition.height = height;
		Invalidate();
	}
	uint32_t OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
		EAGLE_ENG_ASSERT(attachmentIndex < mColorAttachments.size(), "");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		uint32_t pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixelData);
		return pixelData;
	}
	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, uint32_t value) {
		EAGLE_ENG_ASSERT(attachmentIndex < mColorAttachments.size(), "");
		auto& specification = mColorAttachmentSpecifications[attachmentIndex];
		
		glClearTexImage(mColorAttachments[attachmentIndex], 0, FramebufferSpecFormatToGL(specification.format), GL_UNSIGNED_INT, &value);
	}
}