#pragma once
#include <entt.hpp>

// The client will inherit this scene and provide the functions.

namespace Egl {

	class Entity;

	class Scene {
	public:
		Scene();
		virtual ~Scene() = default;

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
	private:
		entt::registry mRegistry;
		entt::entity mPrimaryCamera{ entt::null };

		friend class Entity;
		friend class HierarchyPanel;
	};
}
