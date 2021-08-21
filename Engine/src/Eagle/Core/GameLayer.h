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
		Ref<Scene> GetActiveScene() const { return mActiveScene; }
		void ActivateScene(Ref<Scene> scene);

		Ref<Framebuffer> ReadFramebuffer() const { return mFramebuffer; }
	private:
		Ref<Framebuffer> mFramebuffer;
		Ref<Scene> mActiveScene;
	};
}
