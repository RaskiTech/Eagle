#include "EaglePCH.h"
#include "Eagle/Rendering/Shader.h"

#include "Eagle/Rendering/Renderer.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace Egl {
	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		EAGLE_ENG_ASSERT(!Exists(name), "Shader already exests.");
		mShaders[name] = shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		EAGLE_ENG_ASSERT(Exists(name), "Shader doesn't exests.");
		return mShaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) {
		return mShaders.find(name) != mShaders.end();
	}
}