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
		void RemoveEntity(Entity& entity);
		void SetPrimaryCamera(Entity& camera);
		const Entity GetPrimaryCamera();

		// User-defined functions unique to each instance
		virtual void SceneBegin() = 0;
		virtual void SceneEnd() = 0;

	private:

		void SetViewportAspectRatio(float aspectRatio);
		void OnUpdate();

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

	private:
		inline void AddEntityChildsImp(Entity& createdEntity) {}
		template<typename AddEntityChildParam, typename... AddEntityChildParamRest>
		inline void AddEntityChildsImp(Entity& createdEntity, AddEntityChildParam first, AddEntityChildParamRest&&...rest) {
			createdEntity.AddChild(first);
			AddEntityChildsImp(createdEntity, rest...);
		}
	};
}