#pragma once
#include "Eagle/Core/Core.h"
#include <string>
#include <glm/glm.hpp>

namespace Egl {
	class Shader {
	public:
		static Ref<Shader> Create(const std::string& vectexShader, const std::string& fragmentShader);
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;
	};
}