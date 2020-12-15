#pragma once

#include "Eagle/Layers/Layer.h"
#include "Eagle/Events/KeyEvent.h"
#include "Eagle/Events/MouseEvent.h"
#include "Eagle/Events/ApplicationEvent.h"

namespace Egl {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float mTime = 0.0f;
	};
}
