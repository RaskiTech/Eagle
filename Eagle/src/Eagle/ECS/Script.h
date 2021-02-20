#pragma once
#include "Entity.h"

// Baseclass for all client scripts
namespace Egl {

	class Script {
	public:
		virtual ~Script() = default;

		glm::vec2 ScreenToWorldPos(const glm::vec2& screenPosition) { return GetParentScene()->ScreenToWorldPos(screenPosition); }
		glm::vec2 WorldToScreenPos(const glm::vec2& screenPosition) { return GetParentScene()->WorldToScreenPos(screenPosition); }

		const Entity& GetEntity() const { return mEntity; }
		template<typename T>
		T& GetComponent() const { return mEntity.GetComponent<T>(); }
		template<typename T>
		T& AddComponent()const { return mEntity.AddComponent<T>(); }
		Scene* GetParentScene() const { return mEntity.GetParentScene(); }
		Entity    GetParent() const { return mEntity.GetParent(); }

	private:
		Entity mEntity;
		friend class GameLayer;
	};
}