#pragma once

#include <EaglePCH.h>
#include "Eagle/Rendering/Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Egl {
	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) : mRendererID(0) {
		// Read our shaders into the appropriate buffers

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			LOG_ENG_ERROR("Vertex shader compilation failed. Error: ");
			LOG_ENG_ERROR("{0}", infoLog.data());

			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			LOG_ENG_ERROR("Fragment shader compilation failed. Error: ");
			LOG_ENG_ERROR("{0}", infoLog.data());

			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		mRendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(mRendererID, vertexShader);
		glAttachShader(mRendererID, fragmentShader);

		// Link our program
		glLinkProgram(mRendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(mRendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(mRendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(mRendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			LOG_ENG_ERROR("Shader linking failed. Error: ");
			LOG_ENG_ERROR("{0}", infoLog.data());

			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(mRendererID, vertexShader);
		glDetachShader(mRendererID, fragmentShader);

		LOG_GL_STATUS("Shader init {0}", (unsigned int)mRendererID);
	}
	Shader::~Shader() {
		LOG_GL_STATUS("Shader destroy {0}", (unsigned int)mRendererID);
		glDeleteProgram(mRendererID);
	}

	void Shader::Bind() const {
		LOG_GL_STATUS("Shader bind {0}", (unsigned int)mRendererID);
		glUseProgram(mRendererID);
	}
	void Shader::Unbind() const {
		LOG_GL_STATUS("Shader unbind");
		glUseProgram(0);
	}

	////////////////////////// Set Uniforms //////////////////////////

	void Shader::SetFloat2(const std::string& name, const glm::vec2& values) {
		LOG_GL_STATUS("Shader upload uniform float2 {0}", (unsigned int)mRendererID);
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}
	void Shader::SetFloat3(const std::string& name, const glm::vec3& values) {
		LOG_GL_STATUS("Shader upload uniform float3 {0}", (unsigned int)mRendererID);
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}
	void Shader::SetFloat4(const std::string& name, const glm::vec4& values) {
		LOG_GL_STATUS("Shader upload uniform float4 {0}", (unsigned int)mRendererID);
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}
	void Shader::SetMat4(const std::string& name, const glm::mat4& matrix) {
		LOG_GL_STATUS("Shader upload uniformMat4 {0}", (unsigned int)mRendererID);
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void Shader::SetInt(const std::string& name, int value) {
		LOG_GL_STATUS("Shader upload uniform int {0}", (unsigned int)mRendererID);
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1i(location, value);
	}
}