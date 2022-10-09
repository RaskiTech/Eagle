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
		const SceneRef& GetActiveScene() const { return mActiveScene; }
		void ActivateScene(SceneRef scene);

		Ref<Framebuffer> ReadFramebuffer() const { return mFramebuffer; }
	private:
		Ref<Framebuffer> mFramebuffer;
		SceneRef mActiveScene;
	};
}
