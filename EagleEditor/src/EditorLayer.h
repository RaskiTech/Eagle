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

		virtual void OnUpdate() override;
		void ResetApplication();

		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:

		// ECS fields
		Ref<Scene> mActiveScene;

		// Actual Editor fields
		Ref<FrameBuffer> mFrameBuffer;
		glm::vec2 mScenePanelSize = { 0.0f, 0.0f };
		bool mScenePanelFocused = false, mScenePanelHovered = false;
		HierarchyPanel mHierarchyPanel;
	};
}