#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Eagle/Rendering/Texture.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Components/SceneCamera.h"
#include "Eagle/Core/DoesExist.h"
#include "Eagle/Components/ParticleComponents/ParticleSystem.h"
#include "Eagle/Core/Events/Event.h"

namespace Egl {
	struct TransformComponent {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		float rotation = 0;
		glm::vec2 scale = { 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position) : position(position) {}

		glm::mat4 GetTransform() const {
			return rotation == 0 ?
				glm::translate(glm::mat4(1), position) *                                                    glm::scale(glm::mat4(1), glm::vec3(scale, 1)):
				glm::translate(glm::mat4(1), position) * glm::rotate(glm::mat4(1), rotation, { 0, 0, 1 }) * glm::scale(glm::mat4(1), glm::vec3(scale, 1));
		}

		operator glm::mat4& () { return GetTransform(); }
		operator const glm::mat4& () const { return GetTransform(); }
	};
	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag) : tag(tag) {};

	};

	struct SpriteRendererComponent {
		glm::vec4 color = { 1, 1, 1, 1 };
		Ref<SubTexture> texture = nullptr;
		float tilingFactor = 1;
		
		SpriteRendererComponent() = default;
		SpriteRendererComponent(Ref<Texture> texture, const glm::vec4& color = {1, 1, 1, 1}, float tilingFactor = 1) 
			: color(color), tilingFactor(tilingFactor), texture(CreateRef<SubTexture>(texture, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 })) {}
		SpriteRendererComponent(Ref<SubTexture> texture, const glm::vec4& color = {1, 1, 1, 1}, float tilingFactor = 1) : color(color), texture(texture), tilingFactor(tilingFactor) {}
		SpriteRendererComponent(const glm::vec4& color) : color(color) {  }
	};

	struct CameraComponent {
		SceneCamera camera;
		glm::vec4 backgroundColor = { 0.2f, 0.2f, 0.2f, 1.0f };
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const glm::vec4& backgroundColor, bool fixedAspectRatio = false) : backgroundColor(backgroundColor), fixedAspectRatio(fixedAspectRatio) {}
	};

	struct ParticleSystemComponent {
		Particles::ParticleSystem particleSystem;

		ParticleSystemComponent(uint32_t maxParticlesAmount = 10000)
			: particleSystem(maxParticlesAmount) {}
	};

	struct NativeScriptComponent {
		Script* baseInstance = nullptr;

		std::function<void()> InstantiateFunc;
		std::function<void()> DestroyFunc;

		std::function<void(Script*)> OnCreateFunc;
		std::function<void(Script*)> OnDestroyFunc;
		std::function<void(Script*)> OnUpdateFunc;
		
		template<typename T>
		void Bind() {
			// Figure out how to call the functions
			InstantiateFunc = [&]() {baseInstance = new T(); };
			DestroyFunc = [&]() { delete (T*)baseInstance; };

				
			// COMPILE_IF_VALID is a macro from DoesExist.h
			COMPILE_IF_VALID(T, OnCreate(),
				OnCreateFunc = [](Script* instance) { ((T*)instance)->OnCreate(); }
			);
			COMPILE_IF_VALID(T, OnDestroy(),
				OnDestroyFunc = [](Script* instance) { ((T*)instance)->OnDestroy(); }
			);
			COMPILE_IF_VALID(T, OnUpdate(),
				OnUpdateFunc = [](Script* instance) { ((T*)instance)->OnUpdate(); }
			);

		}
	};
}