#pragma once

#include <EaglePCH.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLShader.h"

namespace Egl {
	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource) {
		EAGLE_PROFILE_FUNCTION();
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

			LOG_ENG_ERROR("{0}", infoLog.data());

			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(mRendererID, vertexShader);
		glDetachShader(mRendererID, fragmentShader);
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