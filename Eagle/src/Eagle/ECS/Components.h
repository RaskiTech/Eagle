#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Eagle/Rendering/Texture.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Components/SceneCamera.h"
#include "Eagle/Core/DoesExist.h"
#include "Eagle/Components/ParticleComponents/ParticleSystem.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/ECS/Entity.h"

namespace Egl {

	struct TransformComponent {
		// Local position is always up to date. If the global position is needed, should be checked if the position if up to date.
		void SetPosition(const glm::vec2& position);
		void SetPosition(float x, float y) { SetPosition({ x, y }); }
		void SetRotation(float rotation);
		void SetScale(const glm::vec2& scale);
		void SetScale(float x, float y) { SetScale({ x, y }); }

		void SetLocalPosition(const glm::vec2& position);
		void SetLocalPosition(float x, float y) { SetLocalPosition({ x, y }); };
		void SetLocalRotation(float rotation);
		void SetLocalScale(const glm::vec2& scale);
		void SetLocalScale(float x, float y) { SetLocalScale({ x, y }); }

		const glm::vec2& GetPosition() const;
		float GetRotation() const;
		const glm::vec2& GetScale() const;

		const glm::vec2& GetLocalPosition() const { return localPosition; }
		float GetLocalRotation() const { return localRotation; }
		const glm::vec2& GetLocalScale() const { return localScale; }

		TransformComponent(const TransformComponent&) = default;
		TransformComponent(Entity entity, const glm::vec3& position) : localPosition(position), thisEntity(entity) {}

		glm::mat4 GetTransform() {
			return GetRotation() == 0 ?
				glm::translate(glm::mat4(1), { GetPosition().x, GetPosition().y, 0 }) *                                                         glm::scale(glm::mat4(1), glm::vec3(GetScale(), 1)):
				glm::translate(glm::mat4(1), { GetPosition().x, GetPosition().y, 0 }) * glm::rotate(glm::mat4(1), GetRotation(), { 0, 0, 1 }) * glm::scale(glm::mat4(1), glm::vec3(GetScale(), 1));
		}

		operator glm::mat4& () { return GetTransform(); }

	private:
		Entity thisEntity;

		glm::vec2 localPosition = { 0.0f, 0.0f };
		float localRotation = 0;
		glm::vec2 localScale = { 1.0f, 1.0f };

		mutable glm::vec2 worldPosition = { 0.0f, 0.0f };
		mutable float worldRotation = 0;
		mutable glm::vec2 worldScale = { 1.0f, 1.0f };

		mutable bool worldPosRight = false;
		mutable bool worldRotRight = false;
		mutable bool worldScaleRight = false;
		friend class Entity;

		void SetWorldPosFlagsFalse(const Relation& thisRel);
		void SetWorldRotFlagsFalse(const Relation& thisRel);
		void SetWorldScaleFlagsFalse(const Relation& thisRel);
	};

	struct MetadataComponent {
		std::string tag;
		int8_t sortingLayer = 0;
		uint8_t subSorting = 0;

		MetadataComponent() = default;
		MetadataComponent(const std::string& tag, uint8_t sortingLayer) : tag(tag), sortingLayer(sortingLayer) {};
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

		//std::function<void()> InstantiateFunc;

		std::function<void(Script*)> OnCreateFunc;
		std::function<void(Script*)> OnDestroyFunc;
		std::function<void(Script*)> OnUpdateFunc;
		
		template<typename T>
		void Bind() {
			baseInstance = new T();
			
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
		void DeleteBase() {
			EAGLE_ENG_ASSERT(baseInstance != nullptr, "baseInstance was null when trying to delete it. Scene unload interrupted previously?");
			delete baseInstance;
		}
	};
}