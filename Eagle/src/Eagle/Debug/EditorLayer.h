#pragma once

#include "Eagle/Core/Application.h"
#include "Eagle/Core/Layers/Layer.h"
#include "Eagle/Core/Events/Event.h"

#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/Framebuffer.h"
#include "Panels/HierarchyPanel.h"


namespace Egl {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;

		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		bool mScenePanelFocused = false, mScenePanelHovered = false;
		HierarchyPanel mHierarchyPanel;
	};
}