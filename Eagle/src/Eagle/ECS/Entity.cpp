#include <EaglePCH.h>
#include "Entity.h"
#include "Components.h"
#include "ComponentsInternal.h"

namespace Egl {
	Entity::Entity(entt::entity entity, Scene* scene) : mEntity(entity), mScene(scene) {}

	void Entity::SetParent(const Entity& parent) const {
		parent.AddChild(*this);
		//Relation& parentRelation = parent.GetComponent<Relation>();
		//Relation& childRelation = mScene->mRegistry.get<Relation>(mEntity);
		//RemoveFromHierarchy(childRelation);
		//
		//EAGLE_ENG_ASSERT(childRelation.parent == entt::null, "Entity already has a parent");
		//
		//if (parentRelation.childrenCount > 0) {
		//	// Get the last child in the parent
		//	entt::entity lastChild = parentRelation.firstChild;
		//	uint8_t i = parentRelation.childrenCount - 1;
		//	while (i > 0) {
		//		EAGLE_ENG_ASSERT(lastChild != entt::null, "Entity doesn't have that many childs");
		//		lastChild = mScene->mRegistry.get<Relation>(lastChild).nextSibling;
		//		i--;
		//	}
		//	EAGLE_ENG_ASSERT(lastChild != entt::null, "Entity doesn't have that many childs");
		//
		//	mScene->mRegistry.get<Relation>(lastChild).nextSibling = mEntity;
		//	childRelation.previousSibling = lastChild;
		//}
		//else {
		//	parentRelation.firstChild = mEntity;
		//}
		//childRelation.parent = parent.mEntity;
		//parentRelation.childrenCount++;
	}

	void Entity::AddChild(const Entity& child) const {
		Relation& parentRelation = GetComponent<Relation>();
		Relation& childRelation = mScene->mRegistry.get<Relation>(child.mEntity);
		RemoveFromHierarchy(childRelation);
		EAGLE_ENG_ASSERT(childRelation.parent == entt::null, "Entity already has a parent");

		if (parentRelation.childrenCount > 0) {
			// Get the last child in the parent
			entt::entity lastChild = parentRelation.firstChild;
			uint8_t i = parentRelation.childrenCount - 1;
			while (i > 0) {
				EAGLE_ENG_ASSERT(lastChild != entt::null, "Entity doesn't have that many childs");
				lastChild = mScene->mRegistry.get<Relation>(lastChild).nextSibling;
				i--;
			}
			EAGLE_ENG_ASSERT(lastChild != entt::null, "Entity doesn't have that many childs");

			mScene->mRegistry.get<Relation>(lastChild).nextSibling = child.mEntity;
			childRelation.previousSibling = lastChild;
		}
		else {
			parentRelation.firstChild = child.mEntity;
		}
		childRelation.parent = mEntity;
		parentRelation.childrenCount++;
	}
	
	void Entity::RemoveFromHierarchy(Relation& entityRelation) const {
		if (entityRelation.nextSibling != entt::null)
			mScene->mRegistry.get<Relation>(entityRelation.nextSibling).previousSibling = entityRelation.previousSibling;
		if (entityRelation.previousSibling != entt::null)
			mScene->mRegistry.get<Relation>(entityRelation.previousSibling).nextSibling = entityRelation.nextSibling;
	}

	Entity Entity::GetParent() const {
		return { GetComponent<Relation>().parent, mScene };
	}

	Entity Entity::GetChild(uint8_t childIndex) const {
		entt::entity wantedEntity = GetComponent<Relation>().firstChild;
		while (childIndex > 0) {
			EAGLE_ENG_ASSERT(wantedEntity != entt::null, "Entity doesn't have that many childs");
			wantedEntity = mScene->mRegistry.get<Relation>(wantedEntity).nextSibling;
			childIndex--;
		}
		return { wantedEntity, mScene };
	}

	bool operator == (const Entity& e1, const Entity& e2) {
		return e1.GetID() == e2.GetID();
	}

}