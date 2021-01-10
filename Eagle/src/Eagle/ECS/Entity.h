#pragma once
#include <entt.hpp>
#include "Eagle/Core/Core.h"
#include "Scene.h"

namespace Egl {
	bool operator == (const Entity& e1, const Entity& e2);

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const entt::entity entity, const Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) const { 
			EAGLE_ENG_ASSERT(!HasComponent<T>(), "Entity already has component");
			return mScene->mRegistry.emplace<T>(mEntity, std::forward<Args>(args)...); 
		}

		template<typename T>
		bool HasComponent() const { 
			EAGLE_ENG_ASSERT(mEntity != entt::null, "The entity doesn't exist, it's null"); 
			return mScene->mRegistry.has<T>(mEntity); 
		}
		
		template<typename T>
		T& GetComponent() const { 
			EAGLE_ENG_ASSERT(HasComponent<T>(), "Entity doesn't have the component");
			return mScene->mRegistry.get<T>(mEntity); 
		}

		template<typename T>
		void RemoveComponent() const {
			EAGLE_ENG_ASSERT(HasComponent<T>(), "Entity doesn't have the component");
			return mScene->mRegistry.remove<T>(entity);
		}

		uint32_t GetID() const { return (uint32_t)mEntity; }
		Scene* GetParentScene() const { return mScene; }

		operator bool() const { return mEntity != entt::null; }

		//std::string ToString() const { 
		//	std::stringstream str;
		//	str << "Entity (" << GetComponent<TagComponent>().tag << " - " << GetID() << ")";
		//	return str.str();
		//}
	private:
		entt::entity mEntity{ entt::null };
		Scene* mScene = nullptr;
	};
}