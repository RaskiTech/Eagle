#pragma once
#include "Entity.h"

// Baseclass for all client scripts
namespace Egl {
	class Script {
	public:
		virtual ~Script() = default;

		glm::vec2               ScreenToWorldPos(const glm::vec2& screenPosition) const { return GetParentScene()->ScreenToWorldPos(screenPosition); }
		glm::vec2               WorldToScreenPos(const glm::vec2& screenPosition) const { return GetParentScene()->WorldToScreenPos(screenPosition); }
		Entity                  AddEntity(const std::string& name, const EntityParams& params) const { return GetParentScene()->AddEntity(name, params); }
		Entity                  AddUIEntity(const std::string& name, const UIEntityParams& params, const Entity& canvasOrParent) const { return GetParentScene()->AddUIEntity(name, params, canvasOrParent); }

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


		void SwitchToScene(SceneRef scene) const { Application::Get().GetGameLayer()->ScheduleSceneSwitch(scene); }

	private:
		Entity mEntity;
		friend class GameLayer;
	};
}