#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Egl {
	class Shader {
	public:
		Shader(const std::string& vectexShader, const std::string& fragmentShader);
		~Shader();

		static Ref<Shader> Create(const std::string& vectexShader, const std::string& fragmentShader) { return std::make_shared<Shader>(vectexShader, fragmentShader); }

		void Bind() const;
		void Unbind() const;


		void SetFloat2(const std::string& name, const glm::vec2& values);
		void SetFloat3(const std::string& name, const glm::vec3& values);
		void SetFloat4(const std::string& name, const glm::vec4& values);
		void SetMat4(const std::string& name, const glm::mat4& matrix);
		void SetInt(const std::string& name, int value);
#ifdef EAGLE_RENDERER_OPENGL
	public:
	private:
		uint32_t mRendererID;
#endif
	};
}