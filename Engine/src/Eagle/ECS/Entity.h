#pragma once
#include <Dependencies/Entt.h>
#include "Eagle/Core/Core.h"
#include "Scene.h"

namespace Egl {
	bool operator == (const Entity& e1, const Entity& e2);
	struct Relation;
	struct MetadataComponent;
	struct Transform;

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
			EAGLE_ENG_ASSERT(mScene->mRegistry.valid(mEntity), "The entity isn't valid. It was probably deleted.");
			return mScene->mRegistry.has<T>(mEntity);
		}
		// Shouldn't be stored for too long since the reference can become
		// invalid if components of the same type are added or removed.
		template<typename T> T& GetComponent() const {
			EAGLE_ENG_ASSERT(HasComponent<T>(), "Entity doesn't have the component");
			return mScene->mRegistry.get<T>(mEntity);
		}
		template<typename T> void RemoveComponent() const {
			EAGLE_ENG_ASSERT(HasComponent<T>(), "Entity doesn't have the component");
			return mScene->mRegistry.remove<T>(mEntity);
		}
		template<> void RemoveComponent<Transform>() const = delete;
		template<> void RemoveComponent<UITransform>() const = delete;
		template<> void RemoveComponent<MetadataComponent>() const = delete;
		template<> void RemoveComponent<Relation>() const = delete;

		Transform& GetTransform() const { return GetComponent<Transform>(); }
		UITransform& GetUITransform() const { return GetComponent<UITransform>(); }

		template<typename ScriptType, typename... Args> ScriptType& AttachScript(Args&&... args) {
			EAGLE_ENG_ASSERT(!HasComponent<NativeScriptComponent>(), "Entity already has a script.");
			return *AddComponent<NativeScriptComponent>().Bind<ScriptType>({ mEntity, mScene }, std::forward<Args>(args)...);
		}
		template<typename ScriptType> ScriptType& GetScript() {
			EAGLE_ENG_ASSERT(HasComponent<NativeScriptComponent>(), "Entity doesn't have a script.");
			return *(ScriptType*)GetComponent<NativeScriptComponent>().baseInstance;
		}

		void SetParent(const Entity& parent) const { parent.AddChild(*this); }
		bool IsValid() const { return mEntity != entt::null; }
		void AddChild(const Entity& child) const;
		Entity GetParent() const;
		Entity GetChild(uint8_t childIndex) const;
		uint32_t GetID() const { return (uint32_t)mEntity; }
		uint32_t GetChildCount() const;
		Scene* GetScene() const { return mScene; }

		template<typename Func>
		void ForEachChild(Func functionThatTakesEntity) const {
			entt::entity entity = GetComponent<Relation>().firstChild;
			while (entity != entt::null) {
				functionThatTakesEntity({ entity, mScene });
				entity = mScene->mRegistry.get<Relation>(entity).nextSibling;
			}
		}


		operator bool() const { return mEntity != entt::null; }

	private:
		entt::entity mEntity = entt::null;
		Scene* mScene = nullptr;
	};
}