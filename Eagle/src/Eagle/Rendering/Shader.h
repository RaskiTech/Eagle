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


#ifdef EAGLE_RENDERER_OPENGL
	public:
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformInt(const std::string& name, int value);
	private:
		uint32_t mRendererID;
#endif
	};
}