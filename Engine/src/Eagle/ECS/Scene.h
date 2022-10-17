#pragma once
#include <Dependencies/Entt.h>
#include <functional>

// The client will inherit this scene and provide the functions.

namespace Egl {

	class Entity;
	class Script;
	struct NativeScriptComponent;
	struct EntityParams;
	struct UIEntityParams;

	class Scene {
	public:
		Scene();
		virtual ~Scene() = default;

		//template<typename... EntityParam>
		//Entity AddEntity(const std::string& name, const EntityParams& params, EntityParam...childs) { auto e = AddEntity(name, params); AddEntityChildsImp(e, childs...); return e; }
		//template<typename... EntityParam>
		//Entity AddEntity(const std::string& name, EntityParam...childs)    { auto e = AddEntity(name); AddEntityChildsImp(e, childs...); return e; }
		Entity AddEntity(const std::string& name, const EntityParams& params, Entity parent);
		Entity AddEntity(const std::string& name, Entity parent);
		Entity AddEntity(const std::string& name, const EntityParams& params);
		Entity AddEntity(const std::string& name);

		Entity AddUIEntity(const std::string& name, const UIEntityParams& params, Entity canvasOrParent);
		Entity AddUIEntity(const std::string& name, Entity canvasOrParent);

		// Adds an entity with the CanvasComponent attached
		Entity AddCanvas();

		void DeleteEntity(Entity& entity);
		void DeleteUIEntity(Entity& entity) { DeleteEntity(entity); }
		void SetPrimaryCamera(Entity& camera);
		const Entity GetPrimaryCamera();
		glm::vec2 Scene::ScreenToWorldPos(const glm::vec2& pixelCoordinate) const;
		glm::vec2 Scene::WorldToScreenPos(const glm::vec2& worldPos) const;

		// User-defined functions unique to each instance
		virtual void SceneBegin() = 0;
		virtual void SceneEnd() = 0;

		void SwitchToScene(SceneRef scene) const;

		enum class SceneState {
			StartedCreate_0     = 0,
			SceneBeginCalled_1  = 1,
			CreateCalled_2      = 2,
			Running_3           = 3,
			StartedDestroying_4 = 4,
			DestroyCalled_5     = 5,
			SceneEndCalled_6    = 6
		};

		SceneState GetSceneState() { return _sceneState; }

		void ValidateSceneCameraExistence() {
			if (!mRegistry.valid(mPrimaryCamera))
				mPrimaryCamera = entt::null;
		}

	private:
		void SetViewportAspectRatio(float aspectRatio);
		void OnUpdate();

		SceneState _sceneState = SceneState::StartedCreate_0;

		void SubscribeToEvents(NativeScriptComponent* script);
		void OptOutOfEvents(NativeScriptComponent* script);
		std::vector<std::pair<Script*, std::function<bool(Script*, Event&)>>> eventScriptsInOrder;

		friend struct NativeScriptComponent;
		friend class GameLayer;

	private:

		std::vector<entt::entity> entityDeleteQueue;

	private:
		entt::registry mRegistry;
		entt::entity mPrimaryCamera = entt::null;

		entt::entity mFirstEntity = entt::null;

		friend class EditorLayer;
		friend class Entity;
		friend class HierarchyPanel;
		friend struct Transform;
		friend struct UITransform;

		void _DeleteEntityNow(entt::entity e);

	};
}