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

		glm::vec2 LocalToWorldPos(const glm::vec2& point) const;
		glm::vec2 WorldToLocalPos(const glm::vec2& point) const;

		TransformComponent(const TransformComponent&) = default;
		TransformComponent(Entity entity, const glm::vec3& position) : localPosition(position), thisEntity(entity) {}

		glm::mat4 GetTransform() const {
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
		friend struct UIAlignComponent;
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
		
		template<typename T, typename ... Args>
		void Bind(Args&&... args) {
			baseInstance = new T(args...);
			
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

	struct CanvasComponent {
		CanvasComponent() {};

	private:
		// Can be removed if some data is added. Entt doesn't like if a component doesn't have any data.
		char dummyDataForEntt;
	};

	struct UIAlignComponent {
		enum class WidthDriver {
			ConstantWidth,    // The object will always have the same width
			RelativeWidth,    // The objects width will be a certain percent of the parents
			AspectWidth       // The width will depend on the height. 1 is height
		};
		enum class HeightDriver {
			ConstantHeight,   // The object will always have the same height
			RelativeHeight,   // The objects height will be a certain percent of the parents
			AspectHeight      // The height will depend on the width. 1 is width
		};
		enum class YDriver {
			PixelsFromTop,    // The objects top will always be the same distance from the windows top
			PixelsFromBottom, // The objects bottom will always be the same distance from the windows bottom
			AlignCenter,     // Have the objects center be at a certain percent vertically
			AlignTop,        // Have the objects top be at a certain percent vertically
			AlignBottom      // Have the objects bottom be at a certain percent vertically
		};
		enum class XDriver {
			PixelsFromLeft,   // The objects left side will always be the same distance from the windows left side
			PixelsFromRight,  // The objects right side will always be the same distance from the windows right side
			AlignCenter,	  // Have the objects center be at a certain percent horizontally
			AlignLeft,       // Have the objects top be at a certain percent horizontally
			AlignRight       // Have the objects right side be at a certain percent horizontally
		};

		UIAlignComponent(Entity thisEntity) : thisEntity(thisEntity) {};
		UIAlignComponent(Entity thisEntity, XDriver xDriver, float xValue, YDriver yDriver, float yValue, WidthDriver widthDriver, float widthValue, HeightDriver heightDriver, float heightValue)
			: thisEntity(thisEntity), xDriver((uint8_t)xDriver), xPosValue(xValue), yDriver((uint8_t)yDriver), yPosValue(yValue),
			  widthDriver((uint8_t)widthDriver), widthValue(widthValue), heightDriver((uint8_t)heightDriver), heightValue(heightValue) {}

		const glm::vec2& GetWorldPosition() const;
		const glm::vec2& GetWorldScale() const;

		XDriver GetXDriver() const { return (XDriver)xDriver; }
		YDriver GetYDriver() const { return (YDriver)yDriver; }
		WidthDriver GetWidthDriver() const { return (WidthDriver)widthDriver; }
		HeightDriver GetHeightDriver() const { return (HeightDriver)heightDriver; }

		float GetXPosValue() const { return xPosValue; }
		float GetYPosValue() const { return yPosValue; }
		float GetWidthValue() const { return widthValue; }
		float GetHeightValue() const { return heightValue; }

		void SetXPosValue(float val);
		void SetYPosValue(float val);
		void SetWidthValue(float val);
		void SetHeightValue(float val);

		void SetXDriver(XDriver driver);
		void SetYDriver(YDriver driver);
		void SetWidthDriver(WidthDriver driver);
		void SetHeightDriver(HeightDriver driver);

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1), { GetWorldPosition().x, GetWorldPosition().y, 0 }) * glm::scale(glm::mat4(1), glm::vec3(GetWorldScale(), 1));
		}
		operator glm::mat4& () { return GetTransform(); }
	protected:
		void SetDimensionFlagsFalse(const Relation& thisRel) const;
		friend struct TransformComponent;
	private:
		void CheckAreDimensionsCorrect() const;
		void CalculateDimensions(const glm::vec2& parentPos, const glm::vec2& parentScale) const;

		uint8_t xDriver      = (uint8_t)XDriver::AlignCenter;
		uint8_t yDriver      = (uint8_t)YDriver::AlignCenter;
		uint8_t widthDriver  = (uint8_t)WidthDriver::RelativeWidth;
		uint8_t heightDriver = (uint8_t)HeightDriver::RelativeHeight;
		float xPosValue = 0, yPosValue = 0, widthValue = 0.8f, heightValue = 0.5f;

		Entity thisEntity;

		mutable bool dimensionsRight = false;
		mutable glm::vec2 worldPosition = { 0, 0 };
		mutable glm::vec2 worldScale = { 1, 1 };
	};
}