#pragma once
#include "Eagle/Core/Core.h"

namespace Egl {

	struct FrameBufferDefenition {
		uint32_t width = 100, height = 100;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;
		static Ref<FrameBuffer> Create(const FrameBufferDefenition& defenition);
		virtual const FrameBufferDefenition& GetDefenition() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetColorAttachementsRendererID() const = 0;
	};

}