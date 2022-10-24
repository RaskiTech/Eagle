#include "EaglePCH.h"
#include "Eagle/Rendering/Shader.h"
#include "Eagle/Core/AssetManager.h"

#include "Eagle/Rendering/Renderer.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace Egl {
	Shader* Shader::CreateDangling(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return new OpenGLShader(filepath);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Shader* Shader::CreateDangling(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
			case RenderAPI::API::None:    EAGLE_ENG_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return new OpenGLShader(name, vertexSrc, fragmentSrc);
		}

		EAGLE_ENG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const ShaderRef& shader) {
		EAGLE_ENG_ASSERT(!Exists(name), "Shader already exests.");
		mShaders[name] = shader;
	}
	ShaderRef ShaderLibrary::Load(const std::string& filepath) {
		ShaderRef shader = Assets::LoadShader(filepath);
		Add(shader);
		return shader;
	}
	ShaderRef ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		ShaderRef shader = Assets::LoadShader(filepath);
		Add(name, shader);
		return shader;
	}
	ShaderRef ShaderLibrary::Get(const std::string& name) {
		EAGLE_ENG_ASSERT(Exists(name), "Shader doesn't exests.");
		return mShaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) {
		return mShaders.count(name) != 0;
	}
}