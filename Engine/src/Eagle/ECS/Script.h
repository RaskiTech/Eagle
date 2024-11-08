#pragma once
#include "Entity.h"
#include "Scene.h"

// Baseclass for all client scripts
namespace Egl {
	class Script {
	public:
		virtual ~Script() = default;

		glm::vec2               ScreenToWorldPos(const glm::vec2& screenPosition) const { return GetScene()->ScreenToWorldPos(screenPosition); }
		glm::vec2               WorldToScreenPos(const glm::vec2& screenPosition) const { return GetScene()->WorldToScreenPos(screenPosition); }

		Entity                  AddEntity(std::string_view name, const EntityParams& params) const                { return GetScene()->AddEntity(name, params); }
		Entity                  AddEntity(std::string_view name, Entity parent, const EntityParams& params) const { return GetScene()->AddEntity(name, parent, params); }
		Entity                  AddEntity(std::string_view name, Entity parent) const                             { return GetScene()->AddEntity(name, parent); }
		Entity                  AddEntity(std::string_view name)                                                  { return GetScene()->AddEntity(name); }

		Entity                  AddUIEntity(std::string_view name, const Entity& canvasOrParent, const UIEntityParams& params) const { return GetScene()->AddUIEntity(name, canvasOrParent, params); }
		Entity                  AddUIEntity(std::string_view name, const Entity& canvasOrParent)                                     { return GetScene()->AddUIEntity(name, canvasOrParent); }

		Entity                  AddCanvas() { return GetScene()->AddCanvas(); }
		void                    DeleteEntity(Entity& entity) { GetScene()->DeleteEntity(entity); }
		void                    DeleteUIEntity(Entity& entity) { GetScene()->DeleteUIEntity(entity); }

		void                    SetParent(const Entity& parent) const { mEntity.SetParent(parent); }
		void                    AddChild(const Entity& child) const { mEntity.AddChild(child); }
		Entity                  GetChild(uint8_t childIndex) const { return mEntity.GetChild(childIndex); }
		uint32_t                GetChildCount() const { return mEntity.GetChildCount(); }

		// Get this script's entity
		const Entity&           GetEntity()       const { return mEntity; } 
		Entity&                 GetEntity()             { return mEntity; }
		template<typename T> T& GetComponent()    const { return mEntity.GetComponent<T>(); }
		template<typename T, typename... Args> T& AddComponent(Args&&... args)    const { return mEntity.AddComponent<T>(std::forward<Args>(args)...); }
		template<typename T> T& HasComponent()    const { return mEntity.HasComponent<T>(); }
		template<typename T> T& RemoveComponent() const { return mEntity.RemoveComponent<T>(); }
		Scene*                  GetScene()  const { return mEntity.GetScene(); }
		Entity                  GetParent()       const { return mEntity.GetParent(); }
		Transform&              GetTransform()    const { return mEntity.GetTransform(); }
		UITransform&            GetUITransform()  const { return mEntity.GetUITransform(); }

		void SwitchToScene(SceneRef scene) const { GetScene()->SwitchToScene(scene); }

	private:
		Entity mEntity;
		friend struct NativeScriptComponent;
	};
}