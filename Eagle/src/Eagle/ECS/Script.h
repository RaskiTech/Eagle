#pragma once
#include "Entity.h"

// Baseclass for all client scripts
namespace Egl {
	class Script {
	public:
		virtual ~Script() = default;

		glm::vec2               ScreenToWorldPos(const glm::vec2& screenPosition) const { return GetParentScene()->ScreenToWorldPos(screenPosition); }
		glm::vec2               WorldToScreenPos(const glm::vec2& screenPosition) const { return GetParentScene()->WorldToScreenPos(screenPosition); }
		Entity                  AddEntity(const std::string& name) const { return GetParentScene()->AddEntity(name); }

		void                    SetParent(const Entity& parent) const { mEntity.SetParent(parent); }
		void                    AddChild(const Entity& child) const { mEntity.AddChild(child); }
		Entity                  GetChild(uint8_t childIndex) const { mEntity.GetChild(childIndex); }

		const Entity&           GetEntity()       const { return mEntity; } 
		template<typename T> T& GetComponent()    const { return mEntity.GetComponent<T>(); }
		template<typename T> T& AddComponent()    const { return mEntity.AddComponent<T>(); }
		template<typename T> T& HasComponent()    const { return mEntity.HasComponent<T>(); }
		template<typename T> T& RemoveComponent() const { return mEntity.RemoveComponent<T>(); }
		Scene*                  GetParentScene()  const { return mEntity.GetParentScene(); }
		Entity                  GetParent()       const { return mEntity.GetParent(); }

	private:
		Entity mEntity;
		friend class GameLayer;
	};
}