#pragma once
#include <Dependencies/GLM.h>
#include "Eagle/Components/SceneCamera.h"
#include "Eagle/Rendering/Texture.h"
#include "Eagle/ECS/Script.h"
#include "Eagle/Components/SceneCamera.h"
#include "Eagle/Core/DoesExist.h"
#include "Eagle/Components/ParticleComponents/ParticleSystem.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/ECS/Entity.h"
#include "Eagle/Rendering/Text/TextRenderer.h"
#include "Eagle/Core/Audio.h"

namespace Egl {
	struct CameraComponent {
		SceneCamera camera;
		glm::vec4 backgroundColor = { 0.2f, 0.2f, 0.2f, 1.0f };
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const glm::vec4& backgroundColor, bool fixedAspectRatio = false) : backgroundColor(backgroundColor), fixedAspectRatio(fixedAspectRatio) {}
	};

	struct UITransform {
	#pragma region drivers
		using Driver = uint8_t;
		enum class LeftDriver {
			Constant = 0 << 0,   // The side will always have the same offset from the parent
			Relative = 1 << 0    // The sides offset will be a certain percent of the parents
		};
		enum class RightDriver {
			Constant = 0 << 4,
			Relative = 1 << 4
		};
		enum class TopDriver {
			Constant = 0 << 0,
			Relative = 1 << 0
		};
		enum class BottomDriver {
			Constant = 0 << 4,
			Relative = 1 << 4
		};

		enum class XDriver {
			Left   = 0 << 0, // The objects left side will always be the same distance from the windows left side
			Center = 1 << 0, // The object center will always be at a certain percent horizontally
			Right  = 2 << 0, // The objects right side will always be the same distance from the windows right side

			// These options are not available. They are confusing and unnecessary.
			//RelativeCenter = 3 << 0, // Have the objects center be at a certain percent horizontally
			//RelativeLeft   = 4 << 0, // Have the objects top be at a certain percent horizontally
			//RelativeRight  = 5 << 0  // Have the objects right side be at a certain percent horizontally
		};
		enum class WidthDriver {
			Constant    = 0 << 4, // The object will always have the same width
			Relative    = 1 << 4, // The objects width will be a certain percent of the parents
			AspectRatio = 2 << 4  // The width will depend on the height. 1 is height
		};
		enum class YDriver {
			Top    = 0 << 0, // The objects top will always be the same distance from the windows top
			Center = 1 << 0, // The objects center will always be at a certain percent vertically
			Bottom = 2 << 0, // The objects bottom will always be the same distance from the windows bottom

			// These options are not available. They are confusing and unnecessary.
			//AlignCenter = 2 << 0, // Have the objects center be at a certain percent vertically
			//AlignTop    = 3 << 0, // Have the objects top be at a certain percent vertically
			//AlignBottom = 4 << 0  // Have the objects bottom be at a certain percent vertically
		};
		enum class HeightDriver {
			Constant    = 0 << 4, // The object will always have the same height
			Relative    = 1 << 4, // The objects height will be a certain percent of the parents
			AspectRatio = 2 << 4  // The height will depend on the width. 1 is width
		};
#pragma endregion

		UITransform(Entity thisEntity) : thisEntity(thisEntity) {};
		UITransform(Entity thisEntity, UITransform::XDriver xDriver, float xValue, UITransform::YDriver yDriver, float yValue, 
			UITransform::WidthDriver widthDriver, float widthValue, UITransform::HeightDriver heightDriver, float heightValue)
			: thisEntity(thisEntity), xBitfield((Driver)xDriver | (Driver)widthDriver), yBitfield((Driver)yDriver | (Driver)heightDriver),
			  xPrimaryValue(xValue), yPrimaryValue(yValue), xSecondaryValue(widthValue), ySecondaryValue(heightValue) {}
		UITransform(Entity thisEntity, Driver xDrivers, Driver yDrivers, float xPrimaryValue, float yPrimaryValue, float xSecondaryValue, float ySecondaryValue, bool useSidesHorizontal, bool useSidesVertical)
			: thisEntity(thisEntity), xBitfield(xDrivers), yBitfield(yDrivers), xPrimaryValue(xPrimaryValue), yPrimaryValue(yPrimaryValue), xSecondaryValue(xSecondaryValue), ySecondaryValue(ySecondaryValue),
			useSidesHorizontal(useSidesHorizontal), useSidesVertical(useSidesVertical) {}

		const glm::vec2& GetWorldPosition() const;
		const glm::vec2& GetWorldScale() const;

		XDriver         GetXDriver()         const { return (XDriver)        (xBitfield & 15 /*00001111*/); }
		YDriver         GetYDriver()         const { return (YDriver)        (yBitfield & 15 /*00001111*/); }
		WidthDriver     GetWidthDriver()     const { return (WidthDriver)    (xBitfield & 240/*11110000*/); }
		HeightDriver    GetHeightDriver()    const { return (HeightDriver)   (yBitfield & 240/*11110000*/); }
		RightDriver     GetRightSideDriver() const { return (RightDriver)    (xBitfield & 240/*11110000*/); }
		LeftDriver      GetLeftSideDriver()  const { return (LeftDriver)     (xBitfield & 15 /*00001111*/); }
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
		void SetLeftSideDriver(LeftDriver driver);
		void SetRightSideDriver(RightDriver driver);
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
		friend struct Transform;
	private:
		void CorrectWorldCoors() const;
		void CalculateDimensions(const glm::vec2& parentPos, const glm::vec2& parentScale) const;

		void CalculateSidesX(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const;
		void CalculateSidesY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const;
		void CalculateScaleXNoAspectCheck(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const;
		void CalculateScaleXAspectCheck(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const;
		void CalculateScaleY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const;
		void CalculatePosX(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const;
		void CalculatePosY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const;

		Entity thisEntity;

		// If the component uses pos and width, xBitField tells x position and width and same goes for y. Then primary value is pos and secondary scale value
		// If the component uses sides, xBitfield tells left and right side and same goes for y. The primary values are left and top and secondary right and bottom.
		bool useSidesHorizontal = false, useSidesVertical = false;
		Driver xBitfield = (Driver)XDriver::Center | (Driver)WidthDriver::Relative;
		Driver yBitfield = (Driver)YDriver::Center | (Driver)HeightDriver::Relative;
		float xPrimaryValue = 0, yPrimaryValue = 0, xSecondaryValue = 0.8f, ySecondaryValue = 0.5f;

		mutable bool dimensionsRight = false;
		mutable glm::vec2 worldPosition = { 0, 0 };
		mutable glm::vec2 worldScale = { 1, 1 };
	};
	struct Transform {
		// Local position is always up to date. If the global position is needed, should be checked if the position if up to date.
		Transform& SetPosition(const glm::vec2& position);
		Transform& SetPosition(float x, float y) { SetPosition({ x, y }); return *this; }
		Transform& SetRotation(float rotation);
		Transform& SetScale(const glm::vec2& scale);
		Transform& SetScale(float x, float y) { SetScale({ x, y }); return *this; }

		Transform& SetLocalPosition(const glm::vec2& position);
		Transform& SetLocalPosition(float x, float y) { SetLocalPosition({ x, y }); return *this; };
		Transform& SetLocalRotation(float rotation);
		Transform& SetLocalScale(const glm::vec2& scale);
		Transform& SetLocalScale(float x, float y) { SetLocalScale({ x, y }); return *this; }

		const glm::vec2& GetPosition() const;
		float GetRotation() const;
		const glm::vec2& GetScale() const;

		const glm::vec2& GetLocalPosition() const { return localPosition; }
		float GetLocalRotation() const { return localRotation; }
		const glm::vec2& GetLocalScale() const { return localScale; }

		glm::vec2 LocalToWorldPos(const glm::vec2& point) const;
		glm::vec2 WorldToLocalPos(const glm::vec2& point) const;

		Transform(const Transform&) = default;
		Transform(Entity entity, const glm::vec2& position) : localPosition(position), thisEntity(entity) {}
		Transform(Entity entity, const glm::vec2& position, float rotation, const glm::vec2& scale) : localPosition(position), localRotation(rotation), localScale(scale), thisEntity(entity) {}

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
		friend struct UITransform;
	};

	struct EntityParams {
		glm::vec2 position = { 0, 0 };
		float rotation = 0;
		glm::vec2 scale = { 1, 1 };

		int8_t sortingLayer = 0;
		uint8_t subSorting = 0;

		EntityParams(int8_t sortingLayer = 0, uint8_t subSorting = 0) : sortingLayer(sortingLayer), subSorting(subSorting) {}
		EntityParams(const glm::vec2& position, float rotation, const glm::vec2& scale, int8_t sortingLayer = 0, uint8_t subSorting = 0)
			: position(position), scale(scale), rotation(rotation), sortingLayer(sortingLayer), subSorting(subSorting) {};
	};
	struct UIEntityParams {

		bool useSidesHorizontal = false, useSidesVertical = false;
		UITransform::Driver xDrivers = (UITransform::Driver)UITransform::XDriver::Center 
			| (UITransform::Driver)UITransform::WidthDriver::Relative;
		UITransform::Driver yDrivers = (UITransform::Driver)UITransform::YDriver::Center 
			| (UITransform::Driver)UITransform::HeightDriver::Relative;
		float xPrimaryValue = 0, yPrimaryValue = 0, xSecondaryValue = 0.8f, ySecondaryValue = 0.5f;

		int8_t sortingLayer = 0;
		uint8_t subSorting = 0;

		UIEntityParams(int8_t sortingLayer = 0, uint8_t subSorting = 0) : sortingLayer(sortingLayer), subSorting(subSorting) {}
		UIEntityParams(UITransform::XDriver xPosition, float xValue, UITransform::WidthDriver width, float widthValue, UITransform::YDriver yPosition, float yValue, UITransform::HeightDriver height, float heightValue, int8_t sortingLayer = 0, uint8_t subSorting = 0)
			: useSidesHorizontal(false), useSidesVertical(false), xDrivers((UITransform::Driver)xPosition | (UITransform::Driver)width), yDrivers((UITransform::Driver)yPosition | (UITransform::Driver)height), 
			  xPrimaryValue(xValue), xSecondaryValue(widthValue), yPrimaryValue(yValue), ySecondaryValue(heightValue), sortingLayer(sortingLayer), subSorting(subSorting) {}
		UIEntityParams(UITransform::LeftDriver left, float leftValue, UITransform::RightDriver right, float rightValue, UITransform::TopDriver top, float topValue, UITransform::BottomDriver bottom, float bottomValue, int8_t sortingLayer = 0, uint8_t subSorting = 0)
			: useSidesHorizontal(true), useSidesVertical(true), xDrivers((UITransform::Driver)left | (UITransform::Driver)right), yDrivers((UITransform::Driver)bottom | (UITransform::Driver)top), 
			  xPrimaryValue(leftValue), xSecondaryValue(rightValue), yPrimaryValue(topValue), ySecondaryValue(bottomValue), sortingLayer(sortingLayer), subSorting(subSorting) {}
		UIEntityParams(UITransform::XDriver xPosition, float xValue, UITransform::WidthDriver width, float widthValue, UITransform::TopDriver top, float topValue, UITransform::BottomDriver bottom, float bottomValue, int8_t sortingLayer = 0, uint8_t subSorting = 0)
			: useSidesHorizontal(false), useSidesVertical(true), xDrivers((UITransform::Driver)xPosition | (UITransform::Driver)width), yDrivers((UITransform::Driver)bottom | (UITransform::Driver)top), 
			  xPrimaryValue(xValue), xSecondaryValue(widthValue), yPrimaryValue(topValue), ySecondaryValue(bottomValue), sortingLayer(sortingLayer), subSorting(subSorting) {}
		UIEntityParams(UITransform::LeftDriver left, float leftValue, UITransform::RightDriver right, float rightValue, UITransform::YDriver yPosition, float yValue, UITransform::HeightDriver height, float heightValue, int8_t sortingLayer = 0, uint8_t subSorting = 0)
			: useSidesHorizontal(true), useSidesVertical(false), xDrivers((UITransform::Driver)left | (UITransform::Driver)right), yDrivers((UITransform::Driver)yPosition | (UITransform::Driver)height), 
			  xPrimaryValue(leftValue), xSecondaryValue(rightValue), yPrimaryValue(yValue), ySecondaryValue(heightValue), sortingLayer(sortingLayer), subSorting(subSorting) {}
	};

	struct MetadataComponent {
		std::string tag;
		int8_t sortingLayer = 0;
		uint8_t subSorting = 0;

		inline uint16_t CalculateSorting() { return ((uint16_t)sortingLayer << 8) + (uint16_t)subSorting; }

		MetadataComponent() = default;
		MetadataComponent(std::string_view tag, int8_t sortingLayer, uint8_t subSorting = 0) : tag(tag), sortingLayer(sortingLayer), subSorting(subSorting) {};
	};

	struct SpriteRendererComponent {
		glm::vec4 color = { 1, 1, 1, 1 };
		SubTextureRef texture = -1;
		float tilingFactor = 1;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(TextureRef texture, const glm::vec4& color = {1, 1, 1, 1}, float tilingFactor = 1) 
			: color(color), tilingFactor(tilingFactor), texture(Assets::LoadSubTexture(texture, glm::vec2{ 0, 0 }, glm::vec2{ 1, 1 })) {}
		SpriteRendererComponent(SubTextureRef texture, const glm::vec4& color = {1, 1, 1, 1}, float tilingFactor = 1) : color(color), texture(texture), tilingFactor(tilingFactor) {}
		SpriteRendererComponent(const glm::vec4& color) : color(color) {  }
	};

	struct TextComponent {
		TextRenderer textRenderer;
		TextProperties props;

		void SetText(std::string_view str) { textRenderer.ChangeRenderedText(str); }
		const std::string& GetText() const { return textRenderer.GetOriginalText(); }

		TextComponent(FontRef font) : textRenderer(font) {}
		TextComponent() = default;
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
		std::function<void(Script*)> OnImGuiFunc;
		std::function<void(Script*)> OnPropertyRenderFunc;
		std::function<bool(Entity, Event&)> OnEventFunc;

		template<typename T, typename ... Args>
		T* Bind(Entity entity, Args&&... args) {
			baseInstance = new T(std::forward<Args>(args)...);
			baseInstance->mEntity = entity;

			COMPILE_IF_VALID(T, OnCreate(),
				OnCreateFunc = [](Script* instance) { ((T*)instance)->OnCreate(); };

			);
			COMPILE_IF_VALID(T, OnDestroy(),
				OnDestroyFunc = [](Script* instance) { ((T*)instance)->OnDestroy(); };
			);
			COMPILE_IF_VALID(T, OnUpdate(),
				OnUpdateFunc = [](Script* instance) { ((T*)instance)->OnUpdate(); };
			);
			COMPILE_IF_VALID(T, OnImGuiRender(),
				OnImGuiFunc = [](Script* instance) { ((T*)instance)->OnImGuiRender(); };
			);
			COMPILE_IF_VALID(T, OnPropertyRender(),
				OnPropertyRenderFunc = [](Script* instance) { ((T*)instance)->OnPropertyRender(); };
			);

			COMPILE_IF_EVENTFUNC(T,
				OnEventFunc = [](Entity instance, Event& e) { return instance.GetScript<T>().OnEvent(e); };
				entity.GetScene()->SubscribeToEvents(this);
			);

			COMPILE_IF_VALID(T, OnCreate(),
				if (entity.GetScene()->GetSceneState() >= Scene::SceneState::SceneBeginCalled_1)
					OnCreateFunc(baseInstance);
			);


			EAGLE_ENG_ASSERT(entity.GetScene()->GetSceneState() < Scene::SceneState::StartedDestroying_4, "Please don't add new components in OnDestroy or in SceneEnd!");

			return (T*)baseInstance;
		}
		void DeleteScript() {
			EAGLE_ENG_ASSERT(baseInstance != nullptr, "baseInstance was null when trying to delete it. Scene unload interrupted previously?");
			if (OnEventFunc)
				baseInstance->mEntity.GetScene()->OptOutOfEvents(this);
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

	struct AudioSource {
		AudioSource(AudioClipRef clip);
		AudioSource(AudioSource& source);
		AudioSource(AudioSource&& source) noexcept;
		AudioSource& operator=(const AudioSource& other);
		~AudioSource();

		void Play(bool play);
		bool IsPlaying() { return sample->playing; }
		void SetVolume(float volume) { sample->volume = glm::clamp(volume, 0.0f, 1.0f); }
		float GetVolume() { return sample->volume; }
		float GetDuration() { return (float)(*sample->clip).data.getLengthInSeconds(); }
		float GetTime() { return (float)sample->samplePosition / (*sample->clip).data.getSampleRate(); }
		void SetTime(float time) { sample->samplePosition = glm::clamp((int)(time * (*sample->clip).data.getSampleRate()), 0, (*sample->clip).data.getNumSamplesPerChannel()); }
		bool GetIsLooping() { return sample->loop; }
		void SetIsLooping(bool loop) { sample->loop = loop; }

		AudioSample* GetSample() { return sample; }

	private:
		void PossibleSampleDelete();
		AudioSample* sample;
	};
}