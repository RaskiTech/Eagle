#pragma once
#include <string>

namespace Egl {
	class Shader {
	public:
		Shader(const std::string& vectexShader, const std::string& fragmentShader);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:

		// Private implementation variables for each renderer

#ifdef EAGLE_RENDERER_OPENGL
		uint32_t mRendererID;
#endif
	};
}