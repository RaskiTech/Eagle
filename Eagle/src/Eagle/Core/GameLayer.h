#pragma once

#include "Eagle/Core/Layers/Layer.h"
#include "Eagle/Core/Events/Event.h"

#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/FrameBuffer.h"


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

		void ResetApplication();
		Ref<Scene> GetActiveScene() const { return mActiveScene; }
	private:
		Ref<Scene> mActiveScene;
	};
}
