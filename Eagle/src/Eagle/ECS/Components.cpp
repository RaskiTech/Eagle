#include "EaglePCH.h"
#include "Components.h"
#include "ComponentsInternal.h"
#include "Entity.h"

namespace Egl {
	// TODO: Rewrite GetPosition(). This is just a temp solution and needs to be changed to something like this:
	// https://github.com/skypjack/entt/issues/643

	void TransformComponent::SetLocalPosition(const glm::vec3& position) {
		localPosition = position;
		SetWorldPosFlagsFalse(thisEntity.GetComponent<Relation>());
	}
	void TransformComponent::SetLocalRotation(float rotation) {
		localRotation = rotation;
		Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldRotFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
	}
	void TransformComponent::SetLocalScale(const glm::vec2& scale) {
		localScale = scale;
		Relation& rel = thisEntity.GetComponent<Relation>();
		SetWorldScaleFlagsFalse(rel);
		SetWorldPosFlagsFalse(rel);
	}
	// Private function
	void TransformComponent::SetWorldPosFlagsFalse(Relation& thisRel) {
		entt::entity child = thisRel.firstChild;
		worldPosRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.SetWorldPosFlagsFalse(childRel);
			child = childRel.nextSibling;
		}
	}
	void TransformComponent::SetWorldRotFlagsFalse(Relation& thisRel) {
		entt::entity child = thisRel.firstChild;
		worldRotRight = false;
		while (child != entt::null) {
			Relation& childRel = thisEntity.GetParentScene()->mRegistry.get<Relation>(child);
			TransformComponent& tr = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(child);
			tr.SetWorldRotFlagsFalse(childRel);
			child = childRel.nextSibling;
		}
	}
	void TransformComponent::SetWorldScaleFlagsFalse(Relation& thisRel) {
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

		entt::entity parent = thisEntity.GetComponent<Relation>().parent;
		worldPosRight = true;
		if (parent != entt::null) {
			TransformComponent& parentTransform = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent);
			glm::vec3 parent_relative_local = { parentTransform.GetScale().x * localPosition.x * glm::cos(parentTransform.GetRotation()) - parentTransform.GetScale().y * localPosition.y * glm::sin(parentTransform.GetRotation()),
				parentTransform.GetScale().x * localPosition.x * glm::sin(parentTransform.GetRotation()) + parentTransform.GetScale().y * localPosition.y * glm::cos(parentTransform.GetRotation()), localPosition.z };
			worldPosition = parentTransform.GetPosition() + parent_relative_local;
		}
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
			worldScale = thisEntity.GetParentScene()->mRegistry.get<TransformComponent>(parent).GetScale() * localScale;
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