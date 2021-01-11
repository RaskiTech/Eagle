#pragma once
#include <entt.hpp>

// Note: Can't include Entity.h

namespace Egl {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene() = default;

		Entity AddEntity(const std::string& name = "New Entity");
		void RemoveEntity(Entity& entity);
		void SetPrimaryCamera(Entity& camera);
		const Entity GetPrimaryCamera();

		void SetViewportAspectRatio(float aspectRatio);

		void OnUpdate();
	private:
		entt::registry mRegistry;
		entt::entity mPrimaryCamera{ entt::null };

		friend class Entity;
		friend class HierarchyPanel;
	};
}
