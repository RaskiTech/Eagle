#pragma once

#include "Eagle/Rendering/FrameBuffer.h"

namespace Egl {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferDefenition& defenition);
		virtual ~OpenGLFrameBuffer();

		virtual const FrameBufferDefenition& GetDefenition() const override { return mDefenition; }

		void Invalidate();
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual uint32_t GetColorAttachementsRendererID() const override { return mColorAttachment; }
	private:
		uint32_t mRendererID = 0;
		uint32_t mColorAttachment, mDepthAttatchment;
		FrameBufferDefenition mDefenition;



	};
}