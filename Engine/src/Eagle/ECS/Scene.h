#pragma once
#include <Dependencies/Entt.h>
#include <functional>
#include "Eagle/Core/UI/UIManager.h"

// The client will inherit this scene and provide the functions.

namespace Egl {

	class Entity;
	class Script;
	struct NativeScriptComponent;
	struct EntityParams;
	struct UIEntityParams;


	class Scene {
	public:

		struct EventCallbackData;
		struct HoverCallbackData;

		Scene();
		virtual ~Scene();

		Entity AddEntity(std::string_view name, const EntityParams& params);
		Entity AddEntity(std::string_view name, Entity parent, const EntityParams& params);
		Entity AddEntity(std::string_view name, Entity parent);
		Entity AddEntity(std::string_view name);

		Entity AddUIEntity(std::string_view name, Entity canvasOrParent, const UIEntityParams& params);
		Entity AddUIEntity(std::string_view name, Entity canvasOrParent);

		// Adds an entity with the CanvasComponent attached
		Entity AddCanvas();

		void DeleteEntity(Entity& entity);
		void DeleteUIEntity(Entity& entity) { DeleteEntity(entity); }
		void SetPrimaryCamera(Entity& camera);
		const Entity GetPrimaryCamera();
		glm::vec2 Scene::ScreenToWorldPos(const glm::vec2& pixelCoordinate) const;
		glm::vec2 Scene::WorldToScreenPos(const glm::vec2& worldPos) const;
		void SubscribeToEvents(Entity instance, std::function<bool(Entity, Event&)> callback);
		void SubscribeToEnterExitEvents(Entity instance, std::function<bool(Entity, Event&)> callback);

		// User-defined functions unique to each instance
		virtual void SceneBegin() = 0;
		virtual void SceneEnd() = 0;

		void SwitchToScene(SceneRef scene) const;

		UIManager& GetUIManager() { return _UIManager; }

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
		void ChangeCameraAspectRatios(float aspectRatio);
		friend class Application;

	private:
		void OnUpdate();

		SceneState _sceneState = SceneState::StartedCreate_0;

		void SubscribeToEvents(NativeScriptComponent* script);
		void OptOutOfEvents(NativeScriptComponent* script);
		std::vector<EventCallbackData> eventCallbacksSorted;
		std::vector<HoverCallbackData> hoverCallbacks;

		friend struct NativeScriptComponent;
		friend class GameLayer;

	private:
		UIManager _UIManager;

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