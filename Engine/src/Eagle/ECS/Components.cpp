#include <EaglePCH.h>
#include <Dependencies/GLM.h>
#include "Components.h"
#include "ComponentsInternal.h"
#include "Eagle/Core/Application.h"
#include "Eagle/Core/AssetManager.h"
#include "Entity.h"

namespace Egl {

	#pragma region TransformComponent
	Transform& Transform::SetPosition(const glm::vec2& position) {
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const glm::vec2& parentWorldPosition = thisEntity.GetScene()->mRegistry.get<Transform>(parent).GetPosition();
			localPosition = position - parentWorldPosition;
		}
		else localPosition = position;

		worldPosition = position;
		worldPosRight = true;

		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldPosFlagsFalse(rel);
		return *this;
	}
	Transform& Transform::SetRotation(float rotation) {
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const float parentWorldRotation = thisEntity.GetScene()->mRegistry.get<Transform>(parent).GetRotation();
			localRotation = rotation - parentWorldRotation;
		}
		else localRotation = rotation;

		worldRotation = rotation;
		worldRotRight = true;

		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldRotFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
		return *this;
	}
	Transform& Transform::SetScale(const glm::vec2& scale) {
		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const glm::vec2& parentWorldScale = thisEntity.GetScene()->mRegistry.get<Transform>(parent).GetScale();
			localScale = scale / parentWorldScale;
		}
		else localScale = scale;

		worldScale = scale;
		worldScaleRight = true;

		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldScaleFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
		return *this;
	}

	Transform& Transform::SetLocalPosition(const glm::vec2& position) {
		localPosition = position;
		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldPosFlagsFalse(rel);
		return *this;
	}
	Transform& Transform::SetLocalRotation(float rotation) {
		localRotation = rotation;
		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldRotFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
		return *this;
	}
	Transform& Transform::SetLocalScale(const glm::vec2& scale) {
		localScale = scale;
		const Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldScaleFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
		return *this;
	}

	const glm::vec2& Transform::GetPosition() const {
		if (!worldPosRight) {
			const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
			if (parent != entt::null) {
				Transform& parentTransform = thisEntity.GetScene()->mRegistry.get<Transform>(parent);
				const glm::vec2 parentAt00Pos = glm::rotateZ(glm::vec3{ parentTransform.GetScale() * localPosition, 0 }, parentTransform.GetRotation());
				worldPosition = parentTransform.GetPosition() + parentAt00Pos;
			}
			else
				worldPosition = localPosition;
			worldPosRight = true;
		}

		return worldPosition;
	}
	float Transform::GetRotation() const {
		if (!worldRotRight) {
			const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
			if (parent != entt::null)
				worldRotation = thisEntity.GetScene()->mRegistry.get<Transform>(parent).GetRotation() + localRotation;
			else
				worldRotation = localRotation;
			worldRotRight = true;
		}

		return worldRotation;
	}
	const glm::vec2& Transform::GetScale() const {
		if (!worldScaleRight) {
			const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
			if (parent != entt::null)
				worldScale = thisEntity.GetScene()->mRegistry.get<Transform>(parent).GetScale() * localScale;
			else
				worldScale = localScale;
			worldScaleRight = true;
		}

		return worldScale;
	}

	void Transform::SetWorldPosFlagsFalse(const Relation& thisRel) {
		// If the flag is already false, all the ones below must be also
		if (!worldPosRight)
			return;

		entt::entity child = thisRel.firstChild;
		worldPosRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetScene()->mRegistry.has<Transform>(child)) {
				Transform& tr = thisEntity.GetScene()->mRegistry.get<Transform>(child);
				tr.SetWorldPosFlagsFalse(childRel);
			}
			else {
				UITransform& align = thisEntity.GetScene()->mRegistry.get<UITransform>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}
	void Transform::SetWorldRotFlagsFalse(const Relation& thisRel) {
		// If the flag is already false, all the ones below must be also
		if (!worldRotRight)
			return;

		entt::entity child = thisRel.firstChild;
		worldRotRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetScene()->mRegistry.has<Transform>(child)) {
				Transform& tr = thisEntity.GetScene()->mRegistry.get<Transform>(child);
				tr.SetWorldRotFlagsFalse(childRel);
			}
			else {
				UITransform& align = thisEntity.GetScene()->mRegistry.get<UITransform>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}
	void Transform::SetWorldScaleFlagsFalse(const Relation& thisRel) {
		// If the flag is already false, all the ones below must be also
		if (!worldScaleRight)
			return;

		entt::entity child = thisRel.firstChild;
		worldScaleRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetScene()->mRegistry.has<Transform>(child)) {
				Transform& tr = thisEntity.GetScene()->mRegistry.get<Transform>(child);
				tr.SetWorldScaleFlagsFalse(childRel);
			}
			else {
				UITransform& align = thisEntity.GetScene()->mRegistry.get<UITransform>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}

	glm::vec2 Transform::LocalToWorldPos(const glm::vec2& point) const {
		Transform& relativeTrans = thisEntity.GetComponent<Transform>();
		const glm::vec2 relativeAt00Pos = glm::rotateZ(glm::vec3{ relativeTrans.GetScale() * point, 0 }, relativeTrans.GetRotation());
		return relativeTrans.GetPosition() + relativeAt00Pos;
	}
	glm::vec2 Transform::WorldToLocalPos(const glm::vec2& point) const {
		Transform& relativeTrans = thisEntity.GetComponent<Transform>();
		const glm::vec2 allDoneBackwards = (glm::vec2)glm::rotateZ(glm::vec3{ point - GetPosition(), 0 }, -relativeTrans.GetRotation()) / relativeTrans.GetScale();
		return allDoneBackwards;
	}
#pragma endregion

	#pragma region UITransformComponent

	// For better ScreenToWorldPos, use Scenes function. These are optimized just for UIAlign
	static float ScreenToWorldPosX(float coor, CameraComponent& camCam, Transform& camTrans) {
		float screenSizeInWorld_X = camCam.camera.GetSize() * camCam.camera.GetAspectRatio();
		return coor / Application::Get().GetSceneWindowSize().x * screenSizeInWorld_X + camTrans.GetPosition().x - screenSizeInWorld_X / 2;
	}
	static inline float ScreenToWorldPosY(float coor, CameraComponent& camCam, Transform& camTrans) {
		return coor / Application::Get().GetSceneWindowSize().y * camCam.camera.GetSize() + camTrans.GetPosition().y - camCam.camera.GetSize() / 2;
	}
	static float WorldToScreenPosX(float coor, CameraComponent& camCam, Transform& camTrans) {
		float screenSizeInWorld_X = camCam.camera.GetSize() * camCam.camera.GetAspectRatio();
		return (coor - camTrans.GetPosition().x + screenSizeInWorld_X / 2) * Application::Get().GetSceneWindowSize().x / screenSizeInWorld_X;
	}
	static inline float WorldToScreenPosY(float coor, CameraComponent& camCam, Transform& camTrans) {
		return (coor - camTrans.GetPosition().y + camCam.camera.GetSize() / 2) * Application::Get().GetSceneWindowSize().y / camCam.camera.GetSize();
	}
	static inline float ScreenToWorldScaleY(float size, CameraComponent& camCam, Transform& camTrans) {
		return size / Application::Get().GetSceneWindowSize().y * camCam.camera.GetSize();
	}
	static inline float ScreenToWorldScaleX(float size, CameraComponent& camCam, Transform& camTrans) {
		return size / Application::Get().GetSceneWindowSize().x * camCam.camera.GetSize() * camCam.camera.GetAspectRatio();
	}
	static inline float WorldToScreenScaleY(float size, CameraComponent& camCam, Transform& camTrans) {
		return size * Application::Get().GetSceneWindowSize().y / camCam.camera.GetSize();
	}
	static inline float WorldToScreenScaleX(float size, CameraComponent& camCam, Transform& camTrans) {
		return size * Application::Get().GetSceneWindowSize().x / (camCam.camera.GetSize() * camCam.camera.GetAspectRatio());
	}

	void UITransform::SetXPosValue(float val) { xPrimaryValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UITransform::SetYPosValue(float val) { yPrimaryValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UITransform::SetWidthValue(float val) { xSecondaryValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }
	void UITransform::SetHeightValue(float val) { ySecondaryValue = val; SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); }

	void UITransform::SetXDriver(XDriver driver) { 
		EAGLE_ENG_WARNING(!useSidesHorizontal, "Called SetXDriver() in UIAlignComponent but its horizontal axis is controlled by sides. Call SetUseSidesHorizontal() to change it or set the sides by their drivers.");
		xBitfield = ((xBitfield & 240/*11110000*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); 
	}
	void UITransform::SetYDriver(YDriver driver) { 
		EAGLE_ENG_WARNING(!useSidesVertical, "Called SetYDriver() in UIAlignComponent but its vertical axis is controlled by sides. Call SetUseSidesHorizontal() to change it or set the sides by their drivers.");
		yBitfield = ((yBitfield & 240/*11110000*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); 
	}
	void UITransform::SetWidthDriver(WidthDriver driver) {
		EAGLE_ENG_WARNING(!useSidesHorizontal, "Called SetWidthDriver() in UIAlignComponent but its horizontal axis is controlled by sides. Call SetUseSidesVertical() to change it or set the sides by their drivers.");
		xBitfield = ((xBitfield & 15/*00001111*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>()); 
	}
	void UITransform::SetHeightDriver(HeightDriver driver) {
		EAGLE_ENG_WARNING(!useSidesVertical, "Called SetHeightDriver() in UIAlignComponent but its vertical axis is controlled by sides. Call SetUseSidesVertical() to change it or set the sides by their drivers.");
		yBitfield = ((yBitfield & 15/*00001111*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>());
	}
	void UITransform::SetLeftSideDriver(LeftDriver driver) {
		EAGLE_ENG_WARNING(useSidesHorizontal, "Called SetLeftSideDriver() in UIAlignComponent but its horizontal axis is controlled by object position and scale. Call SetUseSidesHorizontal() to change it or use the right drivers.");
		xBitfield = ((xBitfield & 240/*11110000*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>());
	}
	void UITransform::SetRightSideDriver(RightDriver driver) {
		EAGLE_ENG_WARNING(useSidesHorizontal, "Called SetRightSideDriver() in UIAlignComponent but its horizontal axis is controlled by object position and scale. Call SetUseSidesHorizontal() to change it or use the right drivers.");
		xBitfield = ((xBitfield & 15/*00001111*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>());
	}
	void UITransform::SetTopDriver(TopDriver driver) {
		EAGLE_ENG_WARNING(useSidesVertical, "Called SetTopDriver() in UIAlignComponent but its vertical axis is controlled by object position and scale. Call SetUseSidesVertical() to change it or use the right drivers.");
		yBitfield = ((yBitfield & 240/*11110000*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>());
	}
	void UITransform::SetBottomDriver(BottomDriver driver) {
		EAGLE_ENG_WARNING(useSidesVertical, "Called SetBottomDriver() in UIAlignComponent but its vertical axis is controlled by object position and scale. Call SetUseSidesVertical() to change it or use the right drivers.");
		yBitfield = ((yBitfield & 15/*00001111*/) | (Driver)driver);
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>());
	}

	void UITransform::SetUseSidesHorizontal(bool sidesOverObjectTransform) {
		useSidesHorizontal = sidesOverObjectTransform;
		xBitfield = 0;
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>());
	}
	void UITransform::SetUseSidesVertical(bool sidesOverObjectTransform) {
		useSidesVertical = sidesOverObjectTransform;
		yBitfield = 0;
		SetDimensionFlagsFalse(thisEntity.GetComponent<Relation>());
	}

	std::pair<const glm::vec2&, const glm::vec2&> UITransform::GetParentWorldCoords(entt::entity parent) const {
		if (thisEntity.GetScene()->mRegistry.has<UITransform>(parent)) {
			UITransform& parentComp = thisEntity.GetScene()->mRegistry.get<UITransform>(parent);
			return std::pair<const glm::vec2&, const glm::vec2&>(parentComp.GetWorldPosition(), parentComp.GetWorldScale());
		}
		else {
			Transform& parentComp = thisEntity.GetScene()->mRegistry.get<Transform>(parent);
			return std::pair<const glm::vec2&, const glm::vec2&>(parentComp.GetPosition(), parentComp.GetScale());
		}
	}

	float UITransform::GetPrimaryXFromWorldPos(float xWorldPos) const {
		Scene* activeScene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
		CameraComponent& cam = activeScene->GetPrimaryCamera().GetComponent<CameraComponent>();
		Transform& camTrans = activeScene->GetPrimaryCamera().GetComponent<Transform>();
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		auto [parentPos, parentScale] = GetParentWorldCoords(parent);

		if (useSidesHorizontal) {
			// xWorldPos is the middle pos. Find the left side pos
			float leftSideWorldPos = xWorldPos - worldScale.x / 2;
			float parentLeftSide = parentPos.x - parentScale.x / 2;
			switch (GetLeftSideDriver()) {
				case LeftDriver::Constant: return WorldToScreenScaleX(leftSideWorldPos - parentLeftSide, cam, camTrans);
				case LeftDriver::Relative: return (leftSideWorldPos - parentLeftSide) / parentScale.x;
			}
		}
		else {
			switch (GetXDriver()) {
				case XDriver::Left:  return WorldToScreenPosX(xWorldPos - (worldScale.x / 2), cam, camTrans) - WorldToScreenPosX(parentPos.x - parentScale.x / 2, cam, camTrans);
				case XDriver::Center:     return (WorldToScreenPosX(xWorldPos, cam, camTrans) - WorldToScreenPosX(parentPos.x, cam, camTrans)) / WorldToScreenScaleX(parentScale.x, cam, camTrans);
				case XDriver::Right: return -(WorldToScreenPosX(xWorldPos + (worldScale.x / 2), cam, camTrans) - Application::Get().GetSceneWindowSize().x + WorldToScreenPosX(parentPos.x - parentScale.x / 2, cam, camTrans));
			}
		}
		return 0;
	}
	float UITransform::GetSecondaryXFromWorldScale(float xWorldScale) const {
		Scene* activeScene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
		CameraComponent& cam = activeScene->GetPrimaryCamera().GetComponent<CameraComponent>();
		Transform& camTrans = activeScene->GetPrimaryCamera().GetComponent<Transform>();
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		auto [parentPos, parentScale] = GetParentWorldCoords(parent);

		if (useSidesHorizontal) {
			// xWorldPos is the middle pos. Find the right side pos
			float rightSideWorldPos = xWorldScale + (worldPosition.x - worldScale.x / 2);
			float parentRightSide = parentPos.x + parentScale.x / 2;
			switch (GetRightSideDriver()) {
				case RightDriver::Constant: return -WorldToScreenScaleX(rightSideWorldPos - parentRightSide, cam, camTrans);
				case RightDriver::Relative: return -(rightSideWorldPos - parentRightSide) / parentScale.x;
			}
		}
		else {
			switch (GetWidthDriver()) {
				case WidthDriver::AspectRatio:   return xWorldScale / worldScale.y;
				case WidthDriver::Constant: return WorldToScreenScaleX(xWorldScale, cam, camTrans);
				case WidthDriver::Relative: return xWorldScale / parentScale.x;
			}
		}
		return 0;
	}
	float UITransform::GetPrimaryYFromWorldPos(float yWorldPos) const {
		Scene* activeScene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
		CameraComponent& cam = activeScene->GetPrimaryCamera().GetComponent<CameraComponent>();
		Transform& camTrans = activeScene->GetPrimaryCamera().GetComponent<Transform>();
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		auto [parentPos, parentScale] = GetParentWorldCoords(parent);

		if (useSidesVertical) {
			// yWorldPos is the middle pos. Find the top pos
			float topWorldPos = yWorldPos + worldScale.y / 2;
			float parentTop = parentPos.y + parentScale.y / 2;
			switch (GetTopDriver()) {
				case TopDriver::Constant: return -WorldToScreenScaleY(topWorldPos - parentTop, cam, camTrans);
				case TopDriver::Relative: return -(topWorldPos - parentTop) / parentScale.y;
			}
		}
		else {
			switch (GetYDriver()) {
				//case YDriver::AlignTop:         return (WorldToScreenPosY(yWorldPos + (worldScale.y / 2), cam, camTrans) - WorldToScreenPosY(parentPos.y, cam, camTrans)) / WorldToScreenScaleY(parentScale.y, cam, camTrans);
				//case YDriver::AlignBottom:      return (WorldToScreenPosY(yWorldPos - (worldScale.y / 2), cam, camTrans) - WorldToScreenPosY(parentPos.y, cam, camTrans)) / WorldToScreenScaleY(parentScale.y, cam, camTrans);
				case YDriver::Top:    return -(WorldToScreenPosY(yWorldPos + (worldScale.y / 2), cam, camTrans) - Application::Get().GetSceneWindowSize().y + WorldToScreenPosY(parentPos.y - parentScale.y / 2, cam, camTrans));
				case YDriver::Center:      return (WorldToScreenPosY(yWorldPos, cam, camTrans) - WorldToScreenPosY(parentPos.y, cam, camTrans)) / WorldToScreenScaleY(parentScale.y, cam, camTrans);
				case YDriver::Bottom: return WorldToScreenPosY(yWorldPos - (worldScale.y / 2), cam, camTrans) - WorldToScreenPosY(parentPos.y - parentScale.y / 2, cam, camTrans);
			}
		}
		return 0;
	}
	float UITransform::GetSecondaryYFromWorldScale(float yWorldScale) const {
		Scene* activeScene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
		CameraComponent& cam = activeScene->GetPrimaryCamera().GetComponent<CameraComponent>();
		Transform& camTrans = activeScene->GetPrimaryCamera().GetComponent<Transform>();
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		auto [parentPos, parentScale] = GetParentWorldCoords(parent);

		if (useSidesVertical) {
			// yWorldPos is the middle pos. Find the bottom pos
			float bottomWorldPos = worldPosition.y - yWorldScale / 2;
			float parentBottom = parentPos.y - parentScale.y / 2;
			switch (GetBottomDriver()) {
				case BottomDriver::Constant: return WorldToScreenScaleY(bottomWorldPos - parentBottom, cam, camTrans);
				case BottomDriver::Relative: return (bottomWorldPos - parentBottom) / parentScale.y;
			}
		}
		else {
			switch (GetHeightDriver()) {
				case HeightDriver::AspectRatio:   return yWorldScale / worldScale.x;
				case HeightDriver::Constant: return WorldToScreenScaleY(yWorldScale, cam, camTrans);
				case HeightDriver::Relative: return yWorldScale / parentScale.y;
			}
		}
		return 0;
	}

	const glm::vec2& UITransform::GetWorldPosition() const {
		if (!dimensionsRight) CorrectWorldCoors();
		return worldPosition;
	}
	const glm::vec2& UITransform::GetWorldScale() const {
		if (!dimensionsRight) CorrectWorldCoors();
		return worldScale;
	}

	void UITransform::SetDimensionFlagsFalse(const Relation& thisRel) const {
		// If the flag is already false, all the ones below must be also
		if (!dimensionsRight)
			return;

		entt::entity child = thisRel.firstChild;
		dimensionsRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetScene()->mRegistry.get<Relation>(child);
			if (thisEntity.GetScene()->mRegistry.has<Transform>(child)) {
				Transform& tr = thisEntity.GetScene()->mRegistry.get<Transform>(child);
				// TODO: Don't change everything, only the thing this functions was called from
				tr.SetWorldPosFlagsFalse(childRel);
				tr.SetWorldRotFlagsFalse(childRel);
				tr.SetWorldScaleFlagsFalse(childRel);
			}
			else {
				UITransform& align = thisEntity.GetScene()->mRegistry.get<UITransform>(child);
				align.SetDimensionFlagsFalse(childRel);
			}
			child = childRel.nextSibling;
		}
	}

	void UITransform::CorrectWorldCoors() const {
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (thisEntity.GetScene()->mRegistry.has<UITransform>(parent)) {
			UITransform& parentComp = thisEntity.GetScene()->mRegistry.get<UITransform>(parent);
			CalculateDimensions(parentComp.GetWorldPosition(), parentComp.GetWorldScale());
		}
		else {
			Transform& parentComp = thisEntity.GetScene()->mRegistry.get<Transform>(parent);
			CalculateDimensions(parentComp.GetPosition(), parentComp.GetScale());
		}
		dimensionsRight = true;
	}
	
	void UITransform::CalculateDimensions(const glm::vec2& parentPos, const glm::vec2& parentScale) const {
		Scene* activeScene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
		CameraComponent& cam = activeScene->GetPrimaryCamera().GetComponent<CameraComponent>();
		Transform& camTrans = activeScene->GetPrimaryCamera().GetComponent<Transform>();

		if (useSidesHorizontal) {
			if (useSidesVertical) {
				CalculateSidesX(parentPos, parentScale, cam, camTrans);
				CalculateSidesY(parentPos, parentScale, cam, camTrans);
			}
			else {
				CalculateSidesX(parentPos, parentScale, cam, camTrans);
		
				CalculateScaleY(parentPos, parentScale, cam, camTrans);
				CalculatePosY(parentPos, parentScale, cam, camTrans);
			}
		}
		else {
			if (useSidesVertical) {
				CalculateSidesY(parentPos, parentScale, cam, camTrans);
		
				CalculateScaleXNoAspectCheck(parentPos, parentScale, cam, camTrans);
				CalculateScaleXAspectCheck(parentPos, parentScale, cam, camTrans);
				CalculatePosX(parentPos, parentScale, cam, camTrans);
			}
			else {
				CalculateScaleXNoAspectCheck(parentPos, parentScale, cam, camTrans);
				CalculateScaleY(parentPos, parentScale, cam, camTrans);
				CalculateScaleXAspectCheck(parentPos, parentScale, cam, camTrans);

				CalculatePosX(parentPos, parentScale, cam, camTrans);
				CalculatePosY(parentPos, parentScale, cam, camTrans);
			}
		}
	}
	inline void UITransform::CalculateSidesX(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const {
		float leftSideWorldPos;
		switch (GetLeftSideDriver()) {
			case LeftDriver::Constant: {
				float parentLeftSide = parentPos.x - parentScale.x / 2;
				leftSideWorldPos = parentLeftSide + ScreenToWorldScaleX(xPrimaryValue, cam, camTrans);
				break;
			}
			case LeftDriver::Relative: {
				float parentLeftSide = parentPos.x - parentScale.x / 2;
				leftSideWorldPos = parentLeftSide + parentScale.x * xPrimaryValue;
				break;
			}
			default: {
				LeftDriver selectedDriver = GetLeftSideDriver();
				EAGLE_ENG_ASSERT(false, "The left side driver wasn't any of the options");
				break;
			}
		}

		float rightSideWorldPos;
		switch (GetRightSideDriver()) {
			case RightDriver::Constant: {
				float parentRightSide = parentPos.x + parentScale.x / 2;
				rightSideWorldPos = parentRightSide - ScreenToWorldScaleX(xSecondaryValue, cam, camTrans);
				break;
			}
			case RightDriver::Relative: {
				float parentRightSide = parentPos.x + parentScale.x / 2;
				rightSideWorldPos = parentRightSide - parentScale.x * xSecondaryValue;
				break;
			}
			default: {
				RightDriver selectedDriver = GetRightSideDriver();
				EAGLE_ENG_ASSERT(false, "The left side driver wasn't any of the options");
				break;
			}
		}

		worldScale.x = rightSideWorldPos - leftSideWorldPos;
		worldPosition.x = (leftSideWorldPos + rightSideWorldPos) / 2;
	}
	inline void UITransform::CalculateSidesY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const {
		float topWorldPos;
		switch (GetTopDriver()) {
			case TopDriver::Constant: {
				float parentTop = parentPos.y + parentScale.y / 2;
				topWorldPos = parentTop - ScreenToWorldScaleY(yPrimaryValue, cam, camTrans);
				break;
			}
			case TopDriver::Relative: {
				float parentTop = parentPos.y + parentScale.y / 2;
				topWorldPos = parentTop - parentScale.y * yPrimaryValue;
				break;
			}
			default: {
				TopDriver selectedDriver = GetTopDriver();
				EAGLE_ENG_ASSERT(false, "The left side driver wasn't any of the options");
				break;
			}
		}

		float bottomWorldPos;
		switch (GetBottomDriver()) {
			case BottomDriver::Constant: {
				float parentBottom = parentPos.y - parentScale.y / 2;
				bottomWorldPos = parentBottom + ScreenToWorldScaleY(ySecondaryValue, cam, camTrans);
				break;
			}
			case BottomDriver::Relative: {
				float parentBottom = parentPos.y - parentScale.y / 2;
				bottomWorldPos = parentBottom + parentScale.y * ySecondaryValue;
				break;
			}
			default: {
				BottomDriver selectedDriver = GetBottomDriver();
				EAGLE_ENG_ASSERT(false, "The left side driver wasn't any of the options");
				break;
			}
		}

		worldScale.y = topWorldPos - bottomWorldPos;
		worldPosition.y = (bottomWorldPos + topWorldPos) / 2;
	}
	inline void UITransform::CalculateScaleXNoAspectCheck(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const {
		switch (GetWidthDriver()) {
			case WidthDriver::Constant: {
				worldScale.x = ScreenToWorldScaleX(xSecondaryValue, cam, camTrans);
				break;
			}
			case WidthDriver::Relative: {
				worldScale.x = xSecondaryValue * parentScale.x;
				break;
			}
		}
	}
	inline void UITransform::CalculateScaleXAspectCheck(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const {
		if (GetWidthDriver() == WidthDriver::AspectRatio)
			worldScale.x = worldScale.y * xSecondaryValue;
	}
	inline void UITransform::CalculateScaleY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const {
		switch (GetHeightDriver()) {
			case HeightDriver::AspectRatio: {
				worldScale.y = worldScale.x * ySecondaryValue;
				break;
			}
			case HeightDriver::Constant: {
				worldScale.y = ScreenToWorldScaleY(ySecondaryValue, cam, camTrans);
				break;
			}
			case HeightDriver::Relative: {
				worldScale.y = ySecondaryValue * parentScale.y, cam, camTrans;
				break;
			}
		}
	}
	inline void UITransform::CalculatePosX(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const {
		switch (GetXDriver()) {
			/*
			case XDriver::AlignLeft: {
				float leftSideOffset = worldScale.x / 2;
				worldPosition.x = xPrimaryValue * parentScale.x + parentPos.x + leftSideOffset;
				break;
			}
			case XDriver::AlignRight: {
				float rightSideOffset = -worldScale.x / 2;
				worldPosition.x = xPrimaryValue * parentScale.x + parentPos.x + rightSideOffset;
				break;
			}
			*/
			case XDriver::Left: {
				float halfOfWidth = worldScale.x / 2;
				worldPosition.x = ScreenToWorldPosX(xPrimaryValue + WorldToScreenPosX(parentPos.x - parentScale.x / 2, cam, camTrans), cam, camTrans) + halfOfWidth;
				break;
			}
			case XDriver::Center: {
				worldPosition.x = xPrimaryValue * parentScale.x + parentPos.x;
				break;
			}
			case XDriver::Right: {
				float halfOfWidth = worldScale.x / 2;
				worldPosition.x = ScreenToWorldPosX(Application::Get().GetSceneWindowSize().x - (xPrimaryValue + WorldToScreenPosX(parentPos.x - parentScale.x / 2, cam, camTrans)), cam, camTrans) - halfOfWidth;
				break;
			}
		}
	}
	inline void UITransform::CalculatePosY(const glm::vec2& parentPos, const glm::vec2& parentScale, CameraComponent& cam, Transform& camTrans) const {
		switch (GetYDriver()) {
			/*
			case YDriver::AlignTop: {
				float topSideOffset = -worldScale.y / 2;
				//worldPosition.y = ScreenToWorldPosY(WorldToScreenScaleY(parentScale.y, cam, camTrans) * yPrimaryValue + WorldToScreenPosY(parentPos.y, cam, camTrans), cam, camTrans) + topSideOffset;
				worldPosition.y = yPrimaryValue * parentScale.y + parentPos.y + topSideOffset;
				break;
			}
			case YDriver::AlignBottom: {
				float bottomSideOffset = worldScale.y / 2;
				//worldPosition.y = ScreenToWorldPosY(WorldToScreenScaleY(parentScale.y, cam, camTrans) * yPrimaryValue + WorldToScreenPosY(parentPos.y, cam, camTrans), cam, camTrans) + bottomSideOffset;
				worldPosition.y = yPrimaryValue * parentScale.y + parentPos.y + bottomSideOffset;
				break;
			}
			*/
			case YDriver::Top: {
				float halfOfHeight = worldScale.y / 2;
				worldPosition.y = ScreenToWorldPosY(Application::Get().GetSceneWindowSize().y - (yPrimaryValue + WorldToScreenPosY(parentPos.y - parentScale.y / 2, cam, camTrans)), cam, camTrans) - halfOfHeight;
				break;
			}
			case YDriver::Center: {
				//worldPosition.y = ScreenToWorldPosY(WorldToScreenScaleY(parentScale.y, cam, camTrans) * yPrimaryValue + WorldToScreenPosY(parentPos.y, cam, camTrans), cam, camTrans);
				worldPosition.y = yPrimaryValue * parentScale.y + parentPos.y;
				break;
			}
			case YDriver::Bottom: {
				float halfOfHeight = worldScale.y / 2;
				worldPosition.y = ScreenToWorldPosY(yPrimaryValue + WorldToScreenPosY(parentPos.y - parentScale.y / 2, cam, camTrans), cam, camTrans) + halfOfHeight;
				break;
			}
		}
	}
	#pragma endregion

	AudioSource::AudioSource(AudioClipRef clip) {
		EAGLE_ENG_ASSERT(clip.Valid(), "Clip reference passed to AudioSource wasn't a valid reference.");
		sample = new AudioSample(clip);
	}
	AudioSource::AudioSource(AudioSource& other) : sample(other.sample) {
		sample->referencesHere++;
	}

	AudioSource::AudioSource(AudioSource&& source) noexcept : sample(source.sample) {
		// No reference management. It only moves
		source.sample = nullptr;
	}
	AudioSource& AudioSource::operator=(const AudioSource& other) {
		sample = other.sample;
		sample->referencesHere++;
		return *this;
	}

	AudioSource::~AudioSource() {
		PossibleSampleDelete();
	}

	void AudioSource::PossibleSampleDelete() {
		if (sample == nullptr)
			return;

		sample->referencesHere--;
		if (sample->referencesHere > 0)
			return;

		if (sample->playing) {
			sample->playing = false;
			AudioPlayer::RemoveSample(sample);
		}

		delete sample;
	}

	void AudioSource::Play(bool play) {

		if (play && !sample->playing) {
			if (sample->samplePosition >= (*sample->clip).data.getNumSamplesPerChannel())
				sample->samplePosition = 0;
			AudioPlayer::AddSample(sample);
		}
		else if (!play && sample->playing)
			AudioPlayer::RemoveSample(sample);

		sample->playing = play;
	}

}