#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Eagle/Components/SceneCamera.h"
#include "Eagle/Rendering/Texture.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Components/SceneCamera.h"
#include "Eagle/Core/DoesExist.h"
#include "Eagle/Components/ParticleComponents/ParticleSystem.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/ECS/Entity.h"
#include "Eagle/Rendering/Text/TextRenderer.h"

namespace Egl {
	struct CameraComponent {
		SceneCamera camera;
		glm::vec4 backgroundColor = { 0.2f, 0.2f, 0.2f, 1.0f };
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const glm::vec4& backgroundColor, bool fixedAspectRatio = false) : backgroundColor(backgroundColor), fixedAspectRatio(fixedAspectRatio) {}
	};

	struct UITransformComponent {
	#pragma region drivers
		using Driver = uint8_t;
		enum class LeftSideDriver {
			ConstOffset = 0 << 0,   // The side will always have the same offset from the parent
			RelativeOffset = 1 << 0    // The sides offset will be a certain percent of the parents
		};
		enum class RightSideDriver {
			ConstOffset = 0 << 4,
			RelativeOffset = 1 << 4
		};
		enum class TopDriver {
			ConstOffset = 0 << 0,
			RelativeOffset = 1 << 0
		};
		enum class BottomDriver {
			ConstOffset = 0 << 4,
			RelativeOffset = 1 << 4
		};

		enum class XDriver {
			ConstLeft   = 0 << 0, // The objects left side will always be the same distance from the windows left side
			ConstRight  = 1 << 0, // The objects right side will always be the same distance from the windows right side
			AlignCenter      = 2 << 0, // Have the objects center be at a certain percent horizontally
			AlignLeft        = 3 << 0, // Have the objects top be at a certain percent horizontally
			AlignRight       = 4 << 0  // Have the objects right side be at a certain percent horizontally
		};
		enum class WidthDriver {
			ConstWidth    = 0 << 4, // The object will always have the same width
			RelativeWidth    = 1 << 4, // The objects width will be a certain percent of the parents
			AspectWidth      = 2 << 4  // The width will depend on the height. 1 is height
		};
		enum class YDriver {
			ConstTop    = 0 << 0, // The objects top will always be the same distance from the windows top
			ConstBottom = 1 << 0, // The objects bottom will always be the same distance from the windows bottom
			AlignCenter      = 2 << 0, // Have the objects center be at a certain percent vertically
			AlignTop         = 3 << 0, // Have the objects top be at a certain percent vertically
			AlignBottom      = 4 << 0  // Have the objects bottom be at a certain percent vertically
		};
		enum class HeightDriver {
			ConstHeight   = 0 << 4, // The object will always have the same height
			RelativeHeight   = 1 << 4, // The objects height will be a certain percent of the parents
			AspectHeight     = 2 << 4  // The height will depend on the width. 1 is width
		};
#pragma endregion

		UITransformComponent(Entity thisEntity) : thisEntity(thisEntity) {};
		UITransformComponent(Entity thisEntity, UITransformComponent::XDriver xDriver, float xValue, UITransformComponent::YDriver yDriver, float yValue, 
			UITransformComponent::WidthDriver widthDriver, float widthValue, UITransformComponent::HeightDriver heightDriver, float heightValue)
			: thisEntity(thisEntity), xBitfield((Driver)xDriver | (Driver)widthDriver), yBitfield((Driver)yDriver | (Driver)heightDriver),
			  xPrimaryValue(xValue), yPrimaryValue(yValue), xSecondaryValue(widthValue), ySecondaryValue(heightValue) {}
		UITransformComponent(Entity thisEntity, Driver xDrivers, Driver yDrivers, float xPrimaryValue, float yPrimaryValue, float xSecondaryValue, float ySecondaryValue, bool useSidesHorizontal, bool useSidesVertical)
			: thisEntity(thisEntity), xBitfield(xDrivers), yBitfield(yDrivers), xPrimaryValue(xPrimaryValue), yPrimaryValue(yPrimaryValue), xSecondaryValue(xSecondaryValue), ySecondaryValue(ySecondaryValue),
			useSidesHorizontal(useSidesHorizontal), useSidesVertical(useSidesVertical) {}

		const glm::vec2& GetWorldPosition() const;
		const glm::vec2& GetWorldScale() const;

		XDriver         GetXDriver()         const { return (XDriver)        (xBitfield & 15 /*00001111*/); }
		YDriver         GetYDriver()         const { return (YDriver)        (yBitfield & 15 /*00001111*/); }
		WidthDriver     GetWidthDriver()     const { return (WidthDriver)    (xBitfield & 240/*11110000*/); }
		HeightDriver    GetHeightDriver()    const { return (HeightDriver)   (yBitfield & 240/*11110000*/); }
		RightSideDriver GetRightSideDriver() const { return (RightSideDriver)(xBitfield & 240/*11110000*/); }
		LeftSideDriver  GetLeftSideDriver()  const { return (LeftSideDriver) (xBitfield & 15 /*00001111*/); }
		TopDriver       GetTopDriver()       const { return (TopDriver)      (yBitfield & 15 /*00001111*/); }
		BottomDriver    GetBottomDriver()    const { return (BottomDriver)   (yBitfield & 240/*11110000*/); }

		float GetXPosValue()       const { return xPrimaryValue; }
		float GetYPosValue()       const { return yPrimaryValue; }
		float GetWidthValue()      const { return xSecondaryValue; }
		float GetHeightValue()     const { return ySecondaryValue; }
		float GetLeftSideValue()   const { return xPrimaryValue; }
		float GetTopValue()        const { return yPrimaryValue; }
		float GetRightSideValue()  const { return xSecondaryValue; }
		float GetBottomValue()     const { return ySecondaryValue; }

		void SetXPosValue(float val);
		void SetYPosValue(float val);
		void SetWidthValue(float val);
		void SetHeightValue(float val);
		inline void SetLeftSideValue(float val) { SetXPosValue(val); }
		inline void SetTopValue(float val) { SetYPosValue(val); }
		inline void SetRightSideValue(float val) { SetWidthValue(val); }
		inline void SetBottomValue(float val) { SetHeightValue(val); }

		void SetXDriver(XDriver driver);
		void SetYDriver(YDriver driver);
		void SetWidthDriver(WidthDriver driver);
		void SetHeightDriver(HeightDriver driver);
		void SetLeftSideDriver(LeftSideDriver driver);
		void SetRightSideDriver(RightSideDriver driver);
		void SetTopDriver(TopDriver driver);
		void SetBottomDriver(BottomDriver driver);

		void SetUseSidesHorizontal(bool sidesOverObjectTransform);
		void SetUseSidesVertical(bool sidesOverObjectTransform);
		bool GetUseSidesHorizontal() const { return useSidesHorizontal; }
		bool GetUseSidesVertical() const { return useSidesVertical; }

		float GetPrimaryXFromWorldPos(float xWorldPos) const;
		float GetSecondaryXFromWorldScale(float xWorldScale) const;
		float GetPrimaryYFromWorldPos(float yWorldPos) const;
		float GetSecondaryYFromWorldScale(float yWorldScale) const;
		std::pair<const glm::vec2&, const glm::vec2&> GetParentWorldCoords(entt::entity parent) const;

		glm::mat4 GetTransform() const { return glm::translate(glm::mat4(1), { GetWorldPosition().x, GetWorldPosition().y, 0 }) * glm::scale(glm::mat4(1), glm::vec3(GetWorldScale(), 1)); }
		operator glm::mat4& () { return GetTransform(); }
	protected:
		void SetDimensionFlagsFalse(const Relation& thisRel) const;
		friend struct TransformComponent;
	private:
		void CorrectWorldCoors() const;
		void CalculateDimensions(const glm::vec2& parentPos, const glm::vec2& parentScale) const;

		void CalculateSidesX(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, TransformComponent& camTrans) const;
		void CalculateSidesY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, TransformComponent& camTrans) const;
		void CalculateScaleXNoAspectCheck(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, TransformComponent& camTrans) const;
		void CalculateScaleXAspectCheck(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, TransformComponent& camTrans) const;
		void CalculateScaleY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, TransformComponent& camTrans) const;
		void CalculatePosX(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, TransformComponent& camTrans) const;
		void CalculatePosY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, TransformComponent& camTrans) const;

		Entity thisEntity;

		// If the component uses pos and width, xBitField tells x position and width and same goes for y. Then primary value is pos and secondary scale value
		// If the component uses sides, xBitfield tells left and right side and same goes for y. The primary values are left and top and secondary right and bottom.
		bool useSidesHorizontal = false, useSidesVertical = false;
		Driver xBitfield = (Driver)XDriver::AlignCenter | (Driver)WidthDriver::RelativeWidth;
		Driver yBitfield = (Driver)YDriver::AlignCenter | (Driver)HeightDriver::RelativeHeight;
		float xPrimaryValue = 0, yPrimaryValue = 0, xSecondaryValue = 0.8f, ySecondaryValue = 0.5f;

		mutable bool dimensionsRight = false;
		mutable glm::vec2 worldPosition = { 0, 0 };
		mutable glm::vec2 worldScale = { 1, 1 };
	};
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
		TransformComponent(Entity entity, const glm::vec2& position) : localPosition(position), thisEntity(entity) {}
		TransformComponent(Entity entity, const glm::vec2& position, float rotation, const glm::vec2& scale) : localPosition(position), localRotation(rotation), localScale(scale), thisEntity(entity) {}

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
		friend struct UITransformComponent;
	};

	struct EntityParams {
		glm::vec2 position = { 0, 0 };
		float rotation = 0;
		glm::vec2 scale = { 1, 1 };

		std::string name = "New entity";
		int8_t sortingLayer = 0;
		uint8_t subSorting = 0;

		EntityParams(const std::string& name = "New entity", int8_t sortingLayer = 0, uint8_t subSorting = 0) : name(name), sortingLayer(sortingLayer), subSorting(subSorting) {}
		EntityParams(const std::string& name, const glm::vec2& position, float rotation, const glm::vec2& scale, int8_t sortingLayer = 0, uint8_t subSorting = 0)
			: name(name), position(position), scale(scale), rotation(rotation), sortingLayer(sortingLayer), subSorting(subSorting) {};
	};
	struct UIEntityParams {

		bool useSidesHorizontal = false, useSidesVertical = false;
		UITransformComponent::Driver xDrivers = (UITransformComponent::Driver)UITransformComponent::XDriver::AlignCenter 
			| (UITransformComponent::Driver)UITransformComponent::WidthDriver::RelativeWidth;
		UITransformComponent::Driver yDrivers = (UITransformComponent::Driver)UITransformComponent::YDriver::AlignCenter 
			| (UITransformComponent::Driver)UITransformComponent::HeightDriver::RelativeHeight;
		float xPrimaryValue = 0, yPrimaryValue = 0, xSecondaryValue = 0.8f, ySecondaryValue = 0.5f;

		std::string name = "New entity";
		int8_t sortingLayer = 0;
		uint8_t subSorting = 0;

		UIEntityParams(const std::string& name = "New entity", int8_t sortingLayer = 0, uint8_t subSorting = 0) : name(name), sortingLayer(sortingLayer), subSorting(subSorting) {}
		UIEntityParams(const std::string& name, UITransformComponent::Driver xDrivers, UITransformComponent::Driver yDrivers, float xPrimaryValue, float yPrimaryValue, float xSecondaryValue, float ySecondaryValue, bool useSidesHorizontal, bool useSidesVertical, int8_t sortingLayer = 0, uint8_t subSorting = 0)
			: name(name), xDrivers(xDrivers), yDrivers(yDrivers), xPrimaryValue(xPrimaryValue), yPrimaryValue(yPrimaryValue), xSecondaryValue(xSecondaryValue), 
			ySecondaryValue(ySecondaryValue), useSidesHorizontal(useSidesHorizontal), useSidesVertical(useSidesVertical), sortingLayer(sortingLayer), subSorting(subSorting) {};
	};

	struct MetadataComponent {
		std::string tag;
		int8_t sortingLayer = 0;
		uint8_t subSorting = 0;

		inline uint16_t CalculateSorting() { return ((uint16_t)sortingLayer << 8) + (uint16_t)subSorting; }

		MetadataComponent() = default;
		MetadataComponent(const std::string& tag, int8_t sortingLayer, uint8_t subSorting = 0) : tag(tag), sortingLayer(sortingLayer), subSorting(subSorting) {};
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

	struct TextComponent {
		TextRenderer renderer;
		TextProperties data;

		void SetText(const std::string& str) { renderer.ChangeRenderedText(str); }
		const std::string& GetText() const { return renderer.GetOriginalText(); }

		TextComponent(const std::string& fontPath) : renderer(fontPath) {}
		TextComponent() : renderer("Assets/Fonts/Roboto/Roboto-Regular.ttf") {}
	};

	struct ParticleSystemComponent {
		Particles::ParticleSystem particleSystem;

		ParticleSystemComponent(uint32_t maxParticlesAmount = 10000)
			: particleSystem(maxParticlesAmount) {}
	};

	struct NativeScriptComponent {
		Script* baseInstance = nullptr;

		std::function<void(Script*)> OnCreateFunc;
		std::function<void(Script*)> OnDestroyFunc;
		std::function<void(Script*)> OnUpdateFunc;
		std::function<bool(Script*, Event&)> OnEventFunc;

		template<typename T, typename ... Args>
		T* Bind(Args&&... args) {
			baseInstance = new T(args...);
			COMPILE_IF_VALID(T, OnCreate(),
				OnCreateFunc = [](Script* instance) { ((T*)instance)->OnCreate(); };
			);
			COMPILE_IF_VALID(T, OnDestroy(),
				OnDestroyFunc = [](Script* instance) { ((T*)instance)->OnDestroy(); };
			);
			COMPILE_IF_VALID(T, OnUpdate(),
				OnUpdateFunc = [](Script* instance) { ((T*)instance)->OnUpdate(); };
			);

			COMPILE_IF_EVENTFUNC(T,
				OnEventFunc = [](Script* instance, Event& e) { return ((T*)instance)->OnEvent(e); };
				Application::Get().GetGameLayer()->SubscribeToEvents(this);
			);

			return (T*)baseInstance;
		}
		void DeleteBase() {
			EAGLE_ENG_ASSERT(baseInstance != nullptr, "baseInstance was null when trying to delete it. Scene unload interrupted previously?");
			delete baseInstance;
		}
	};

	struct CanvasComponent {
		#pragma warning( push )
		#pragma warning( disable: 26495 ) // always initialize
		CanvasComponent() {};

	private:
		// Can be removed if some data is added. Entt doesn't like if a component doesn't have any data.
		char dummyDataForEntt;
		#pragma warning( pop )
	};


}