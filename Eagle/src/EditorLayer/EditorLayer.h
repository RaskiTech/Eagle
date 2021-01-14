#pragma once

#include "Eagle/Core/Application.h"
#include "Eagle/Core/Layers/Layer.h"
#include "Eagle/Events/Event.h"

#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/FrameBuffer.h"
#include "Panels/HierarchyPanel.h"


namespace Egl {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void PreUpdate();
		virtual void OnUpdate() override;
		void PostUpdate();

		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		// Actual Editor fields
		glm::vec2 mScenePanelSize = { 0.0f, 0.0f };
		bool mScenePanelFocused = false, mScenePanelHovered = false;
		HierarchyPanel mHierarchyPanel;
	};
}