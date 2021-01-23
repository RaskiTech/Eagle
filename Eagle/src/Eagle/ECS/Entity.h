#pragma once
#include <Dependencies/Entt.h>
#include "Eagle/Core/Core.h"
#include "Scene.h"

namespace Egl {
	bool operator == (const Entity& e1, const Entity& e2);
	struct Relation;

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const entt::entity entity, const Scene* scene);

		template<typename T, typename... Args> T& AddComponent(Args&&... args) const {
			EAGLE_ENG_ASSERT(!HasComponent<T>(), "Entity already has component");
			return mScene->mRegistry.emplace<T>(mEntity, std::forward<Args>(args)...);
		}
		template<typename T> bool HasComponent() const {
			EAGLE_ENG_ASSERT(mEntity != entt::null, "The entity doesn't exist, it's null");
			return mScene->mRegistry.has<T>(mEntity);
		}
		template<typename T> T& GetComponent() const {
			EAGLE_ENG_ASSERT(HasComponent<T>(), "Entity doesn't have the component");
			return mScene->mRegistry.get<T>(mEntity);
		}
		template<typename T> void RemoveComponent() const {
			EAGLE_ENG_ASSERT(HasComponent<T>(), "Entity doesn't have the component");
			EAGLE_ENG_ASSERT(constexpr T == TransformComponent, "Tried to remove TransformComponent");
			EAGLE_ENG_ASSERT(constexpr T == Relation, "Tried to remove RelationComponent");
			EAGLE_ENG_ASSERT(constexpr T == TagComponent, "Tried to remove TagComponent");

			return mScene->mRegistry.remove<T>(entity);
		}

		void SetParent(const Entity& parent) const;
		void AddChild(const Entity& child) const;
		Entity GetParent() const;
		Entity GetChild(uint8_t childIndex) const;

		uint32_t GetID() const { return (uint32_t)mEntity; }
		Scene* GetParentScene() const { return mScene; }

		operator bool() const { return mEntity != entt::null; }
	private:
		void RemoveFromHierarchy(Relation& entityRelation) const;

		entt::entity mEntity = entt::null;
		Scene* mScene = nullptr;
	};
}