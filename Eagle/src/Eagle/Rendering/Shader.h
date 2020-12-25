#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Egl {
	class Shader {
	public:
		Shader(const std::string& vectexShader, const std::string& fragmentShader);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const glm::mat4& matrix, const std::string& name);

#ifdef EAGLE_RENDERER_OPENGL
	private:
		uint32_t mRendererID;
#endif
	};
}