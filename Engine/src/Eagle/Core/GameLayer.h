#pragma once

#include "Eagle/Core/Events/Event.h"

#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/Framebuffer.h"


namespace Egl {

	class GameLayer {
	public:
		GameLayer();
		~GameLayer() = default;

		void OnAttach();
		void OnDetach();

		void OnUpdate(bool needToDrawToScreen);

		void OnEvent(Event& event);
		void OnImGuiRender();

		void DistributeEvent(Event& e);
		void SubscribeToEvents(NativeScriptComponent* script);
		void OptOutOfEvents(NativeScriptComponent* script);

		void ResetApplication();
		const SceneRef& GetActiveScene() const { return _activeScene; }
		void ActivateScene(SceneRef scene);
		void DeactivateCurrentScene();
		void ScheduleSceneSwitch(SceneRef scene) { _scheduledSceneSwitch = scene; }

		Ref<Framebuffer> ReadFramebuffer() const { return _framebuffer; }
	private:
		Ref<Framebuffer> _framebuffer;
		SceneRef _activeScene;
		SceneRef _scheduledSceneSwitch;
	};
}
