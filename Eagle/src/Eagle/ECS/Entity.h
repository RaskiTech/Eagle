#pragma once
#include <Dependencies/Entt.h>
#include "Eagle/Core/Core.h"
#include "Scene.h"

namespace Egl {
	bool operator == (const Entity& e1, const Entity& e2);
	struct Relation;
	struct TagComponent;
	struct TransformComponent;

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
			return mScene->mRegistry.remove<T>(mEntity);
		}
		template<> void RemoveComponent<TransformComponent>() const = delete;
		template<> void RemoveComponent<TagComponent>() const = delete;
		template<> void RemoveComponent<Relation>() const = delete;

		void SetParent(const Entity& parent) const { parent.AddChild(*this); }
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