#pragma once
#include <EaglePCH.h>
#include "Eagle/Core/Core.h"

namespace Egl {
	enum class FramebufferTextureFormat {
		None,

		RGBA8,
		RED_INT,

		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification(FramebufferTextureFormat format) : format(format) {}
		FramebufferTextureFormat format = FramebufferTextureFormat::None;
	};
	struct FramebufferAttachments {
		FramebufferAttachments() = default;
		FramebufferAttachments(std::initializer_list<FramebufferTextureSpecification> attachments)
			: attachments(attachments) {}
		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferDefenition {
		FramebufferAttachments attachments;
		uint32_t width = 100, height = 100;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;
		static Ref<Framebuffer> Create(const FramebufferDefenition& defenition);
		static bool IsDepthFormat(const FramebufferTextureFormat& format);

		virtual void ClearAttachment(uint32_t attachmentIndex, uint32_t value) = 0;

		virtual const FramebufferDefenition& GetDefenition() const = 0;
		virtual uint32_t GetColorAttachementsRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void DrawToScreenAndUnbind(uint32_t screenWidth, uint32_t screenHeight) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
	};
}