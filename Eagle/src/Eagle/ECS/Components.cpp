#include <EaglePCH.h>
#include <glm/gtx/rotate_vector.hpp>
#include "Components.h"
#include "ComponentsInternal.h"
#include "Eagle/Core/Application.h"
#include "Entity.h"

namespace Egl {

	//// TransformComponent ////

	void TransformComponent::SetPosition(const glm::vec2& position) {
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const glm::vec2& parentWorldPosition = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetPosition();
			localPosition = position - parentWorldPosition;
		}
		else localPosition = position;

		worldPosition = position;
		worldPosRight = true;

		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldPosFlagsFalse(rel);
	}
	void TransformComponent::SetRotation(float rotation) {
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const float parentWorldRotation = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetRotation();
			localRotation = rotation - parentWorldRotation;
		}
		else localRotation = rotation;

		worldRotation = rotation;
		worldRotRight = true;

		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldRotFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
	}
	void TransformComponent::SetScale(const glm::vec2& scale) {
		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const glm::vec2& parentWorldScale = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetScale();
			localScale = scale - parentWorldScale;
		}
		else localScale = scale;

		worldScale = scale;
		worldScaleRight = true;

		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldScaleFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
	}

	void TransformComponent::SetLocalPosition(const glm::vec2& position) {
		localPosition = position;
		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldPosFlagsFalse(rel);
	}
	void TransformComponent::SetLocalRotation(float rotation) {
		localRotation = rotation;
		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldRotFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
	}
	void TransformComponent::SetLocalScale(const glm::vec2& scale) {
		localScale = scale;
		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldScaleFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
	}

	const glm::vec2& TransformComponent::GetPosition() const {
		if (!worldPosRight) {
			const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
			if (parent != entt::null) {
				TransformComponent& parentTransform = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent);
				const glm::vec2 parentAt00Pos = glm::rotateZ(glm::vec3{ parentTransform.GetScale() * localPosition, 0 }, parentTransform.GetRotation());
				worldPosition = parentTransform.GetPosition() + parentAt00Pos;
			}
			else
				worldPosition = localPosition;
			worldPosRight = true;
		}

		return worldPosition;
	}
	float TransformComponent::GetRotation() const {
		if (!worldRotRight) {
			const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
			if (parent != entt::null)
				worldRotation = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetRotation() + localRotation;
			else
				worldRotation = localRotation;
			worldRotRight = true;
		}

		return worldRotation;
	}
	const glm::vec2& TransformComponent::GetScale() const {
		if (!worldScaleRight) {
			const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
			if (parent != entt::null)
				worldScale = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetScale() * localScale;
			else
				worldScale = localScale;
			worldScaleRight = true;
		}

		return worldScale;
	}

	void TransformComponent::SetWorldPosFlagsFalse(const Relation& thisRel) {
		// If the flag is already false, all the ones below must be also
		if (!worldPosRight)
			return;

		entt::entity child = thisRel.firstChild;
		worldPosRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetParentScene()->mRegistry.has<TransformComponent>(child)) {
				TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
				tr.SetWorldPosFlagsFalse(childRel);
			}
			else {
				UIAlignComponent& align = thisEntity.GetParentScene()->mRegistry.get<UIAlignComponent>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}
	void TransformComponent::SetWorldRotFlagsFalse(const Relation& thisRel) {
		// If the flag is already false, all the ones below must be also
		if (!worldRotRight)
			return;

		entt::entity child = thisRel.firstChild;
		worldRotRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetParentScene()->mRegistry.has<TransformComponent>(child)) {
				TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
				tr.SetWorldRotFlagsFalse(childRel);
			}
			else {
				UIAlignComponent& align = thisEntity.GetParentScene()->mRegistry.get<UIAlignComponent>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}
	void TransformComponent::SetWorldScaleFlagsFalse(const Relation& thisRel) {
		// If the flag is already false, all the ones below must be also
		if (!worldScaleRight)
			return;

		entt::entity child = thisRel.firstChild;
		worldScaleRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetParentScene()->mRegistry.has<TransformComponent>(child)) {
				TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
				tr.SetWorldScaleFlagsFalse(childRel);
			}
			else {
				UIAlignComponent& align = thisEntity.GetParentScene()->mRegistry.get<UIAlignComponent>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}

	glm::vec2 TransformComponent::LocalToWorldPos(const glm::vec2& point) const {
		TransformComponent& relativeTrans = thisEntity.GetComponent<TransformComponent>();
		const glm::vec2 relativeAt00Pos = glm::rotateZ(glm::vec3{ relativeTrans.GetScale() * point, 0 }, relativeTrans.GetRotation());
		return relativeTrans.GetPosition() + relativeAt00Pos;
	}
	glm::vec2 TransformComponent::WorldToLocalPos(const glm::vec2& point) const {
		TransformComponent& relativeTrans = thisEntity.GetComponent<TransformComponent>();
		const glm::vec2 allDoneBackwards = (glm::vec2)glm::rotateZ(glm::vec3{ point - GetPosition(), 0 }, -relativeTrans.GetRotation()) / relativeTrans.GetScale();
		return allDoneBackwards;
	}

	//// UIAlignComponent ////

	// For better ScreenToWorldPos, use Scenes function. These are just for UIAlign
	static float ScreenToWorldPosX(float coor, CameraComponent& camCam, TransformComponent& camTrans) {
		float screenSizeInWorld_X = camCam.camera.GetSize() * camCam.camera.GetAspectRatio();
		return coor / Application::Get().GetSceneWindowSize().x * screenSizeInWorld_X + camTrans.GetPosition().x - screenSizeInWorld_X / 2;
	}
	static inline float ScreenToWorldPosY(float coor, CameraComponent& camCam, TransformComponent& camTrans) {
		return coor / Application::Get().GetSceneWindowSize().y * camCam.camera.GetSize() + camTrans.GetPosition().y - camCam.camera.GetSize() / 2;
	}
	static float WorldToScreenPosX(float coor, CameraComponent& camCam, TransformComponent& camTrans) {
		float screenSizeInWorld_X = camCam.camera.GetSize() * camCam.camera.GetAspectRatio();
		return (coor - camTrans.GetPosition().x + screenSizeInWorld_X / 2) * Application::Get().GetSceneWindowSize().x / screenSizeInWorld_X;
	}
	static inline float WorldToScreenPosY(float coor, CameraComponent& camCam, TransformComponent& camTrans) {
		return (coor - camTrans.GetPosition().y + camCam.camera.GetSize() / 2) * Application::Get().GetSceneWindowSize().y / camCam.camera.GetSize();
	}
	static inline float ScreenToWorldScaleY(float size, CameraComponent& camCam, TransformComponent& camTrans) {
		return size / Application::Get().GetSceneWindowSize().y * camCam.camera.GetSize();
	}
	static inline float ScreenToWorldScaleX(float size, CameraComponent& camCam, TransformComponent& camTrans) {
		return size / Application::Get().GetSceneWindowSize().x * camCam.camera.GetSize() * camCam.camera.GetAspectRatio();
	}
	static inline float WorldToScreenScaleY(float size, CameraComponent& camCam, TransformComponent& camTrans) {
		return size * Application::Get().GetSceneWindowSize().y / camCam.camera.GetSize();
	}
	static inline float WorldToScreenScaleX(float size, CameraComponent& camCam, TransformComponent& camTrans) {
		return size * Application::Get().GetSceneWindowSize().x / (camCam.camera.GetSize() * camCam.camera.GetAspectRatio());
	}

	void UIAlignComponent::SetXPosValue(float val) { xPosValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UIAlignComponent::SetYPosValue(float val) { yPosValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UIAlignComponent::SetWidthValue(float val) { widthValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UIAlignComponent::SetHeightValue(float val) { heightValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }

	void UIAlignComponent::SetXDriver(XDriver driver) { xDriver = (uint8_t)driver; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UIAlignComponent::SetYDriver(YDriver driver) { yDriver = (uint8_t)driver; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UIAlignComponent::SetWidthDriver(WidthDriver driver) { widthDriver = (uint8_t)driver; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UIAlignComponent::SetHeightDriver(HeightDriver driver) { heightDriver = (uint8_t)driver; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }

	const glm::vec2& UIAlignComponent::GetWorldPosition() const {
		CheckAreDimensionsCorrect();
		return worldPosition;
	}
	const glm::vec2& UIAlignComponent::GetWorldScale() const {
		CheckAreDimensionsCorrect();
		return worldScale;
	}

	void UIAlignComponent::SetDimensionFlagsFalse(const Relation& thisRel) const {
		// If the flag is already false, all the ones below must be also
		if (!dimensionsRight)
			return;

		entt::entity child = thisRel.firstChild;
		dimensionsRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetParentScene()->mRegistry.has<TransformComponent>(child)) {
				TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
				// TODO: Don't change everything, only the thing this functions was called from
				tr.SetWorldPosFlagsFalse(childRel);
				tr.SetWorldRotFlagsFalse(childRel);
				tr.SetWorldScaleFlagsFalse(childRel);
			}
			else {
				UIAlignComponent& align = thisEntity.GetParentScene()->mRegistry.get<UIAlignComponent>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}

	void UIAlignComponent::CheckAreDimensionsCorrect() const {
		if (!dimensionsRight) {
			const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
			if (thisEntity.GetParentScene()->mRegistry.has<UIAlignComponent>(parent)) {
				UIAlignComponent& parentComp = thisEntity.GetParentScene()->mRegistry.get<UIAlignComponent>(parent);
				CalculateDimensions(parentComp.GetWorldPosition(), parentComp.GetWorldScale());
			}
			else {
				TransformComponent& parentComp = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent);
				CalculateDimensions(parentComp.GetPosition(), parentComp.GetScale());
			}
			dimensionsRight = true;
		}
	}
	void UIAlignComponent::CalculateDimensions(const glm::vec2& parentPos, const glm::vec2& parentScale) const {
		Ref<Scene> activeScene = Application::Get().GetGameLayer()->GetActiveScene();
		CameraComponent& cam = activeScene->GetPrimaryCamera().GetComponent<CameraComponent>();
		TransformComponent& camTrans = activeScene->GetPrimaryCamera().GetComponent<TransformComponent>();
		// TODO: Maybe multiply scale by parents scale so that the world size is relative to it's parent

		/////// Scale ///////
		switch ((WidthDriver)widthDriver) {
			case WidthDriver::ConstantWidth: {
				worldScale.x = ScreenToWorldScaleX(widthValue, cam, camTrans);
				break;
			}
			case WidthDriver::RelativeWidth: {
				worldScale.x = widthValue * parentScale.x, cam, camTrans;
				break;
			}
		}

		switch ((HeightDriver)heightDriver) {
			case HeightDriver::AspectHeight: {
				worldScale.y = worldScale.x * heightValue;
				break;
			}
			case HeightDriver::ConstantHeight: {
				worldScale.y = ScreenToWorldScaleY(heightValue, cam, camTrans);
				break;
			}
			case HeightDriver::RelativeHeight: {
				worldScale.y = heightValue * parentScale.y, cam, camTrans;
				break;
			}
		}

		if ((WidthDriver)widthDriver == WidthDriver::AspectWidth)
			worldScale.x = worldScale.y * widthValue;

		/////// Position ///////
		// Note: values are likely in worldSpace and not screen space

		switch ((XDriver)xDriver) {
			case XDriver::AlignCenter: {
				worldPosition.x = ScreenToWorldPosX(WorldToScreenScaleX(parentScale.x, cam, camTrans) * xPosValue + WorldToScreenPosX(parentPos.x, cam, camTrans), cam, camTrans);
				break;
			}
			case XDriver::AlignLeft: {
				float leftSideOffset = worldScale.x / 2;
				worldPosition.x = ScreenToWorldPosX(WorldToScreenScaleX(parentScale.x, cam, camTrans) * xPosValue + WorldToScreenPosX(parentPos.x, cam, camTrans), cam, camTrans) + leftSideOffset;
				break;
			}
			case XDriver::AlignRight: {
				float rightSideOffset = -worldScale.x / 2;
				worldPosition.x = ScreenToWorldPosX(WorldToScreenScaleX(parentScale.x, cam, camTrans) * xPosValue + WorldToScreenPosX(parentPos.x, cam, camTrans), cam, camTrans) + rightSideOffset;
				break;
			}
			case XDriver::PixelsFromLeft: {
				float halfOfWidth = worldScale.x / 2;
				worldPosition.x = ScreenToWorldPosX(xPosValue + WorldToScreenPosX(parentPos.x - parentScale.x / 2, cam, camTrans), cam, camTrans) + halfOfWidth;
				break;
			}
			case XDriver::PixelsFromRight: {
				float halfOfWidth = worldScale.x / 2;
				worldPosition.x = ScreenToWorldPosX(Application::Get().GetSceneWindowSize().x - (xPosValue + WorldToScreenPosX(parentPos.x - parentScale.x / 2, cam, camTrans)), cam, camTrans) - halfOfWidth;
				break;
			}
		}

		switch ((YDriver)yDriver) {
			case YDriver::AlignCenter: {
				worldPosition.y = ScreenToWorldPosY(WorldToScreenScaleY(parentScale.y, cam, camTrans) * yPosValue + WorldToScreenPosY(parentPos.y, cam, camTrans), cam, camTrans);
				break;
			}
			case YDriver::AlignTop: {
				float topSideOffset = -worldScale.y / 2;
				worldPosition.y = ScreenToWorldPosY(WorldToScreenScaleY(parentScale.y, cam, camTrans) * yPosValue + WorldToScreenPosY(parentPos.y, cam, camTrans), cam, camTrans) + topSideOffset;
				break;
			}
			case YDriver::AlignBottom: {
				float bottomSideOffset = worldScale.y / 2;
				worldPosition.y = ScreenToWorldPosY(WorldToScreenScaleY(parentScale.y, cam, camTrans) * yPosValue + WorldToScreenPosY(parentPos.y, cam, camTrans), cam, camTrans) + bottomSideOffset;
				break;
			}
			case YDriver::PixelsFromTop: {
				float halfOfHeight = worldScale.y / 2;
				worldPosition.y = ScreenToWorldPosY(Application::Get().GetSceneWindowSize().y - (yPosValue + WorldToScreenPosY(parentPos.y - parentScale.y / 2, cam, camTrans)), cam, camTrans) - halfOfHeight;
				break;
			}
			case YDriver::PixelsFromBottom: {
				float halfOfHeight = worldScale.y / 2;
				worldPosition.y = ScreenToWorldPosY(yPosValue + WorldToScreenPosY(parentPos.y - parentScale.y / 2, cam, camTrans), cam, camTrans) + halfOfHeight;
				break;
			}
		}
	}
}