#include <EaglePCH.h>
#include <glm/gtx/rotate_vector.hpp>
#include "Components.h"
#include "ComponentsInternal.h"
#include "Entity.h"

namespace Egl {
	void TransformComponent::SetLocalPosition(const glm::vec3& position) {
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
	void TransformComponent::SetWorldPosFlagsFalse(const Relation& thisRel) {
		// If the flag is already false, all the ones below must be also
		if (!worldPosRight)
			return;

		entt::entity child = thisRel.firstChild;
		worldPosRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.SetWorldPosFlagsFalse(childRel);
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
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.SetWorldRotFlagsFalse(childRel);
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
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.SetWorldScaleFlagsFalse(childRel);
			child = childRel.nextSibling;
		}
	}

	const glm::vec3& TransformComponent::GetPosition() {
		if (worldPosRight)
			return worldPosition;

		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		worldPosRight = true;
		if (parent != entt::null) {
			TransformComponent& parentTransform = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent);
			const glm::vec3 parent_relative_local = glm::rotateZ(glm::vec3{ parentTransform.GetScale().x, parentTransform.GetScale().y, 1 } * localPosition, parentTransform.GetRotation());
			worldPosition = parentTransform.GetPosition() + parent_relative_local;
		}
		else
			worldPosition = localPosition;

		return worldPosition;
	}
	float TransformComponent::GetRotation() {
		if (worldRotRight)
			return worldRotation;

		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
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

		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		worldScaleRight = true;
		if (parent != entt::null)
			worldScale = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetScale() * localScale;
		else
			worldScale = localScale;

		return worldScale;
	}

	void TransformComponent::SetPosition(const glm::vec3& position) {
		const entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		if (parent != entt::null) {
			const glm::vec3& parentWorldPosition = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetPosition();
			localPosition = position - parentWorldPosition;
		}
		else localPosition = position;

		worldPosition = position;
		worldPosRight = true;
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