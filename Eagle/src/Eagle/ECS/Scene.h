#pragma once
#include <Dependencies/Entt.h>

// The client will inherit this scene and provide the functions.


namespace Egl {

	class Entity;

	class Scene {
	public:
		Scene();
		virtual ~Scene() = default;

		template<typename... EntityParam>
		Entity AddEntity(const std::string& name, EntityParam...childs) { auto e = AddEntity(name); AddEntityChildsImp(e, childs...); return e; }
		Entity AddEntity(const std::string& name = "New Entity");

		Entity AddUIEntity(const std::string& name, Entity CanvasOrUIParent);

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

		bool areEntitiesInOrder = false;
		std::vector<entt::entity> entitiesInSortOrder;

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

	private:
		inline void AddEntityChildsImp(Entity& createdEntity) {}
		template<typename AddEntityChildParam, typename... AddEntityChildParamRest>
		inline void AddEntityChildsImp(Entity& createdEntity, AddEntityChildParam first, AddEntityChildParamRest&&...rest) {
			createdEntity.AddChild(first);
			AddEntityChildsImp(createdEntity, rest...);
		}
	};
}