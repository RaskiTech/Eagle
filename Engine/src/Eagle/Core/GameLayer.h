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

		void ResetApplication();
		const SceneRef& GetActiveScene() const { return _ActiveScene; }
		void ActivateScene(SceneRef scene);
		void DeactivateCurrentScene();
		void ScheduleSceneSwitch(SceneRef scene) { _ScheduledSceneSwitch = scene; }

		Ref<Framebuffer> ReadFramebuffer() const { return _Framebuffer; }
	private:
		Ref<Framebuffer> _Framebuffer;
		SceneRef _ActiveScene;
		SceneRef _ScheduledSceneSwitch;
	};
}
