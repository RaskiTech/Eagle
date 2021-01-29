#include "EaglePCH.h"
#include "Components.h"
#include "ComponentsInternal.h"
#include "Entity.h"

namespace Egl {
	void TransformComponent::SetLocalPosition(const glm::vec3& position) {
		localPosition = position;
		SetWorldPosFalse(thisEntity.GetComponent<Relation>());
	}
	void TransformComponent::SetLocalRotation(float rotation) {
		localRotation = rotation;
		SetWorldRotFalse(thisEntity.GetComponent<Relation>());
	}
	void TransformComponent::SetLocalScale(const glm::vec2& scale) {
		localScale = scale;
		SetWorldScaleFalse(thisEntity.GetComponent<Relation>());
	}
	// Private function
	void TransformComponent::SetWorldPosFalse(Relation& thisRel) {
		entt::entity child = thisRel.firstChild;
		worldPosRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.worldPosRight = false;
			tr.SetWorldPosFalse(childRel);
			child = childRel.nextSibling;
		}
	}
	void TransformComponent::SetWorldRotFalse(Relation& thisRel) {
		entt::entity child = thisRel.firstChild;
		worldRotRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.worldRotRight = false;
			tr.SetWorldRotFalse(childRel);
			child = childRel.nextSibling;
		}
	}
	void TransformComponent::SetWorldScaleFalse(Relation& thisRel) {
		entt::entity child = thisRel.firstChild;
		worldScaleRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.worldScaleRight = false;
			tr.SetWorldScaleFalse(childRel);
			child = childRel.nextSibling;
		}
	}

	const glm::vec3& TransformComponent::GetPosition() {
		if (worldPosRight)
			return worldPosition;

		LOG_WARN("CHANGING WODL POS");
		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		worldPosRight = true;
		if (parent != entt::null)
			worldPosition = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetPosition() + localPosition;
		else
			worldPosition = localPosition;

		return worldPosition;
	}
	float TransformComponent::GetRotation() {
		if (worldRotRight)
			return worldRotation;

		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		worldRotRight = true;
		if (parent != entt::null)
			worldRotation = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetRotation() + localRotation;
		else
			worldRotation = localRotation;

		return worldRotation;
	}

	const glm::vec2& TransformComponent::GetScale() {
		if (worldScaleRight)
			return worldScale;

		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		worldScaleRight = true;
		if (parent != entt::null)
			worldScale = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetScale() + localScale;
		else
			worldScale = localScale;

		return worldScale;
	}

	void TransformComponent::SetPosition(const glm::vec3& position) {
		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const glm::vec3& parentWorldPosition = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetPosition();
			localPosition = position - parentWorldPosition;
		}
		else localPosition = position;

		worldPosition = position;
		worldPosRight = true;
	}
	void TransformComponent::SetRotation(float rotation) {
		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			float parentWorldRotation = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetRotation();
			localRotation = rotation - parentWorldRotation;
		}
		else localRotation = rotation;

		worldRotation = rotation;
		worldRotRight = true;
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
	}
}