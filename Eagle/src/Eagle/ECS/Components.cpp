#include <EaglePCH.h>
#include <glm/gtx/rotate_vector.hpp>
#include "Components.h"
#include "ComponentsInternal.h"
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

	const glm::vec2& UIAlignComponent::GetPosition() const {
		// TODO: make the function
		int i = 1 / 0;
	}
	const glm::vec2& UIAlignComponent::GetScale() const {
		// TODO: make the function
		int i = 1 / 0;
	}
}