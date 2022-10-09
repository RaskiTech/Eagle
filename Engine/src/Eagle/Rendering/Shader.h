#pragma once
#include <string>
#include <Dependencies/GLM.h>
#include <unordered_map>
#include "Eagle/Core/Core.h"
#include "Eagle/Core/AssetManager.h"

namespace Egl {
	class Shader {
	public:
		static Shader* CreateDangling(const std::string& filepath);
		static Shader* CreateDangling(const std::string& name, const std::string& vectexShader, const std::string& fragmentShader);
		virtual ~Shader() = default;

		virtual const std::string& GetName() const = 0;

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

	// Currently not used, but this exists
	class ShaderLibrary {
	public:
		void Add(const std::string& name, const ShaderRef& shader);
		inline void Add(const ShaderRef& shader) { Add(Assets::GetShader(shader)->GetName(), shader); }
		ShaderRef Load(const std::string& filepath);
		ShaderRef Load(const std::string& name, const std::string& filepath);

		ShaderRef Get(const std::string& name);
		bool Exists(const std::string& name);
	private:
		std::unordered_map<std::string, ShaderRef> mShaders;
	};
}