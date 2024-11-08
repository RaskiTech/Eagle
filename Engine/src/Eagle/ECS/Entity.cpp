#include <EaglePCH.h>
#include "Entity.h"
#include "Components.h"
#include "ComponentsInternal.h"

namespace Egl {
	Entity::Entity(entt::entity entity, Scene* scene) : mEntity(entity), mScene(scene) {}

	void Entity::AddChild(const Entity& child) const {

		Relation& parentRelation = GetComponent<Relation>();
		Relation& childRelation = mScene->mRegistry.get<Relation>(child.mEntity);
		EAGLE_ENG_ASSERT(parentRelation.childCount != 255, "Entity already has the maximum amount of childs, 255");

		if (child.HasComponent<Transform>()) {
			auto& childTransform = child.GetComponent<Transform>();
			childTransform.worldPosRight = false;
			childTransform.worldRotRight = false;
			childTransform.worldScaleRight = false;
		}

		EAGLE_ENG_ASSERT(childRelation.parent == entt::null, "Entity already has a parent");

		// Remove the child from the hierarchy
		if (mScene->mFirstEntity == child.mEntity)
			mScene->mFirstEntity = childRelation.nextSibling;
		if (childRelation.nextSibling != entt::null)
			mScene->mRegistry.get<Relation>(childRelation.nextSibling).previousSibling = childRelation.previousSibling;
		if (childRelation.previousSibling != entt::null)
			mScene->mRegistry.get<Relation>(childRelation.previousSibling).nextSibling = childRelation.nextSibling;

		if (parentRelation.childCount > 0) {
			// Set the parentRelation.firstChild point to the new child and the new child point to the previous first child
			childRelation.nextSibling = parentRelation.firstChild;
			mScene->mRegistry.get<Relation>(parentRelation.firstChild).previousSibling = child.mEntity;
			parentRelation.firstChild = child.mEntity;
		}
		else {
			parentRelation.firstChild = child.mEntity;
			childRelation.nextSibling = entt::null;
			childRelation.previousSibling = entt::null;
		}
		childRelation.parent = mEntity;
		parentRelation.childCount++;
	}

	Entity Entity::GetParent() const {
		return { GetComponent<Relation>().parent, mScene };
	}

	Entity Entity::GetChild(uint8_t childIndex) const {
		Relation& rel = GetComponent<Relation>();
		if (childIndex >= rel.childCount) {
			LOG_ENG_ERROR("Child index was greater than child count. Entity doesn't have that many childs.");
			return Entity();
		}

		entt::entity wantedEntity = rel.firstChild;
		while (childIndex > 0) {
			EAGLE_ENG_ASSERT(wantedEntity != entt::null, "Entity doesn't have that many childs");
			wantedEntity = mScene->mRegistry.get<Relation>(wantedEntity).nextSibling;
			childIndex--;
		}
		return { wantedEntity, mScene };
	}

	uint32_t Entity::GetChildCount() const {
		return GetComponent<Relation>().childCount;
	}

	bool operator == (const Entity& e1, const Entity& e2) {
		return e1.GetID() == e2.GetID();
	}

}