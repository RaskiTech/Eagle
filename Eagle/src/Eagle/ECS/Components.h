#pragma once
#include <glm/glm.hpp>
#include "Eagle/Rendering/Texture.h"
#include "Eagle/ECS/Script.h"
#include "SceneCamera.h"
#include "Eagle/Core/DoesExist.h"
#include "Eagle/Components/ParticleSystem.h"

namespace Egl {
	struct TransformComponent {
		glm::mat4 transform = glm::mat4(1);

		const glm::vec3& GetPosition() { return transform[3]; }
		void SetPosition(const glm::vec3& position) { transform[3] = glm::vec4(position, 0); }

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : transform(transform) {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
	};
	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag) : tag(tag) {};

	};

	struct SpriteComponent {
		glm::vec4 color = { 1, 1, 1, 1 };
		//Texture texture;

		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) : color(color) {}
	};

	struct CameraComponent {
		SceneCamera camera;
		glm::vec4 backgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const glm::vec4& backgroundColor, bool fixedAspectRatio = false) : backgroundColor(backgroundColor), fixedAspectRatio(fixedAspectRatio) {}
	};

	struct ParticleSystemComponent {
		ParticleSystem particleSystem;

		float timeBetweenEmits = 0.1f;
		float timeUntilEmit = 0;

		ParticleSystemComponent() = default;
		ParticleSystemComponent(ParticleSystemProps& props, uint32_t particleAmount = 10000, float timeBetweenEmits = 0.1f)
			: particleSystem(props, particleAmount), timeBetweenEmits(timeBetweenEmits) {}
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