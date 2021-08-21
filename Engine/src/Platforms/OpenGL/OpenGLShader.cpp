#pragma once
#include <EaglePCH.h>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLShader.h"

namespace Egl {
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")                      return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		EAGLE_ENG_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {
		std::string source = ReadFile(filepath);
		auto shaders = ProcessShaderFileText(source);
		Compile(shaders);

		// Get name
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastIndex = filepath.rfind(".");
		auto count = lastIndex == std::string::npos ? filepath.size() - lastSlash : lastIndex - lastSlash;
		mName = filepath.substr(lastSlash, count);
	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) 
	: mName(name) {
		std::unordered_map<GLenum, std::string> shaders;
		shaders[GL_VERTEX_SHADER] = vertexSource;
		shaders[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shaders);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (!in) {
			LOG_ENG_ERROR("Couldn't open file '{0}'", filepath);
			return "";
		}

		in.seekg(0, std::ios::end);
		std::string result;
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();

		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::ProcessShaderFileText(const std::string& source) {
		std::unordered_map<GLenum, std::string> shaders;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos) {
			size_t endOfFile = source.find_first_of("\r\n", pos);
			EAGLE_ENG_ASSERT(endOfFile != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, endOfFile - begin);
			EAGLE_ENG_ASSERT(ShaderTypeFromString(type), "Invalid shader type.")

			size_t nextLinePos = source.find_first_not_of("\r\n", endOfFile);
			pos = source.find(typeToken, nextLinePos);
			shaders[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size()-1 : nextLinePos));
		}

		return shaders;
	}
	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaders) {
		EAGLE_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		EAGLE_ENG_ASSERT(shaders.size() <= 2, "Eagle currntly supports only 2 shaders");
		std::array<GLenum, 2> glShaderIDs;
		int shaderIdIndex = 0;
		for (auto& kv : shaders) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCstr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				LOG_ENG_ERROR("{0}", infoLog.data());
				EAGLE_ENG_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[shaderIdIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);

			for (auto id : glShaderIDs) 
				glDeleteShader(id);

			LOG_ENG_ERROR("{0}", infoLog.data());
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		mRendererID = program;
	}

	OpenGLShader::~OpenGLShader() {
		EAGLE_PROFILE_FUNCTION();
		glDeleteProgram(mRendererID);
	}

	void OpenGLShader::Bind() const {
		EAGLE_PROFILE_FUNCTION();
		glUseProgram(mRendererID);
	}
	void OpenGLShader::Unbind() const {
		EAGLE_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	////////////////////////// Set Uniforms //////////////////////////

	void OpenGLShader::SetInt(const std::string& name, int value) {
		EAGLE_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
		EAGLE_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1iv(location, count, values);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value) {
		EAGLE_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values) {
		EAGLE_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values) {
		EAGLE_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values) {
		EAGLE_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
		EAGLE_PROFILE_FUNCTION();
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}