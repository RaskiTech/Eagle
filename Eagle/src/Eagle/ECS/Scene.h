#pragma once
#include <Dependencies/Entt.h>
#include <Eagle/Rendering/Text/TextRenderer.h>

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
		Entity AddEntity(const EntityParams& params, EntityParam...childs) { auto e = AddEntity(params); AddEntityChildsImp(e, childs...); return e; }
		template<typename... EntityParam>
		Entity AddEntity(const std::string& name, EntityParam...childs)    { auto e = AddEntity(name  ); AddEntityChildsImp(e, childs...); return e; }
		Entity AddEntity(const EntityParams& params);
		Entity AddEntity(const std::string& name);

		Entity AddUIEntity(const UIEntityParams& params, Entity canvasOrParent);
		Entity AddUIEntity(const std::string& name, Entity canvasOrParent);

		// Adds an entity with the CanvasComponent attached
		Entity AddCanvas();

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

		inline void AddEntityChildsImp(Entity& createdEntity) {}
		template<typename AddEntityChildParam, typename... AddEntityChildParamRest>
		inline void AddEntityChildsImp(Entity& createdEntity, AddEntityChildParam first, AddEntityChildParamRest&&...rest) {
			createdEntity.AddChild(first);
			AddEntityChildsImp(createdEntity, rest...);
		}
	};
}