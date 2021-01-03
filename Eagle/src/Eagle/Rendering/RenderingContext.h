#pragma once

namespace Egl {
	class RenderingContext {
	public:
		virtual ~RenderingContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}