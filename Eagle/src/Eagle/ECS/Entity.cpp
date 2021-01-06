#include <EaglePCH.h>
#include "Entity.h"

namespace Egl {
	Entity::Entity(entt::entity entity, Scene* scene) : mEntity(entity), mScene(scene) {}

	bool operator == (const Entity& e1, const Entity& e2) {
		return e1.GetID() == e2.GetID();
	}

}