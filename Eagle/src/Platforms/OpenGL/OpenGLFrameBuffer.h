#pragma once
#include "Eagle/Core/Core.h";
#include "Eagle/Rendering/Framebuffer.h"

namespace Egl {
	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferDefenition& defenition);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual const FramebufferDefenition& GetDefenition() const override { return mDefenition; }
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual uint32_t ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, uint32_t value) override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual uint32_t GetColorAttachementsRendererID(uint32_t index = 0) const override { EAGLE_ENG_ASSERT(index < mColorAttachments.size(), "Index bigger than size"); return mColorAttachments[index]; }
	private:
		uint32_t mRendererID = 0;
		FramebufferDefenition mDefenition;

		std::vector<FramebufferTextureSpecification> mColorAttachmentSpecifications;
		FramebufferTextureSpecification mDepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> mColorAttachments;
		uint32_t mDepthAttachment;
	};
}