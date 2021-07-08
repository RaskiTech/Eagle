#pragma once

#include "Eagle/Core/Layers/Layer.h"
#include "Eagle/Core/Events/Event.h"

#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/Framebuffer.h"


namespace Egl {

	class GameLayer : public Layer {
	public:
		GameLayer();
		~GameLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;

		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

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
