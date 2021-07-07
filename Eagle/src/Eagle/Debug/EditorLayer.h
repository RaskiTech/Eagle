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

		void PreUpdate();
		virtual void OnUpdate() override;
		void PostUpdate();

		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		// Actual Editor fields
		Ref<Framebuffer> mFramebuffer;
		bool mScenePanelFocused = false, mScenePanelHovered = false;
		HierarchyPanel mHierarchyPanel;
	};
}