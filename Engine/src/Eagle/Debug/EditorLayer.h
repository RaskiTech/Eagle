#pragma once
#include "Eagle/Core/Application.h"
#include "Eagle/Core/Events/Event.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/Framebuffer.h"
#include "Panels/HierarchyPanel.h"


namespace Egl {

	class EditorLayer {
	public:
		EditorLayer();
		~EditorLayer() = default;

		void OnAttach();
		void OnDetach();

		void OnUpdate();
		void DrawSelectedEntityOutline(const CameraComponent& camera);

		void OnEvent(Event& event);
		void OnImGuiRender();

	private:
		bool mScenePanelFocused = false, mScenePanelHovered = false;
		HierarchyPanel mHierarchyPanel;
	};
}