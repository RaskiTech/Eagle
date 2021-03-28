#pragma once
#include <Dependencies/Entt.h>

// The client will inherit this scene and provide the functions.


namespace Egl {

	class Entity;
	struct NativeScriptComponent;
	struct EntityParams;
	struct UIEntityParams;

	class Scene {
	public:
		Scene();
		virtual ~Scene() = default;

		template<typename... EntityParam>
		Entity AddEntity(const EntityParams& entity, EntityParam...childs) { auto e = AddEntity(entity); AddEntityChildsImp(e, childs...); return e; }
		Entity AddEntity(const EntityParams& entity);
		Entity AddCanvas();
		Entity AddUIEntity(const UIEntityParams& entity, Entity CanvasOrParent);

		void RemoveEntity(Entity& entity);
		void SetPrimaryCamera(Entity& camera);
		const Entity GetPrimaryCamera();
		glm::vec2 Scene::ScreenToWorldPos(const glm::vec2& pixelCoordinate) const;
		glm::vec2 Scene::WorldToScreenPos(const glm::vec2& worldPos) const;

		// User-defined functions unique to each instance
		virtual void SceneBegin() = 0;
		virtual void SceneEnd() = 0;

	private:
		void SetViewportAspectRatio(float aspectRatio);
		void OnUpdate();

		bool sceneInitComplete = false;
		std::vector<NativeScriptComponent*> eventScriptsInOrder;

		friend struct NativeScriptComponent;
		friend class GameLayer;
		friend class EditorLayer;
		friend class Application;
	private:
		entt::registry mRegistry;
		entt::entity mPrimaryCamera = entt::null;

		entt::entity mFirstEntity = entt::null;

		friend class Entity;
		friend class HierarchyPanel;
		friend struct TransformComponent;
		friend struct UIAlignComponent;
		friend struct NativeScriptComponent;

	private:
		inline void AddEntityChildsImp(Entity& createdEntity) {}
		template<typename AddEntityChildParam, typename... AddEntityChildParamRest>
		inline void AddEntityChildsImp(Entity& createdEntity, AddEntityChildParam first, AddEntityChildParamRest&&...rest) {
			createdEntity.AddChild(first);
			AddEntityChildsImp(createdEntity, rest...);
		}
	};
}