#pragma once

#include "Eagle/Core/Application.h"
#include "Eagle/Core/Layers/Layer.h"
#include "Eagle/Core/Events/Event.h"

#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/Framebuffer.h"
#include "Panels/HierarchyPanel.h"


namespace Egl {

	class EditorLayer {
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach();
		virtual void OnDetach();

		virtual void OnUpdate();

		virtual void OnEvent(Event& event);
		virtual void OnImGuiRender();

	private:
		bool mScenePanelFocused = false, mScenePanelHovered = false;
		HierarchyPanel mHierarchyPanel;
	};
}