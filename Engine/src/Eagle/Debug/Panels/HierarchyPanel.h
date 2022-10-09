#pragma once
#include "Eagle/ECS/Scene.h"
#include "Eagle/Core/Core.h"
#include "Eagle/ECS/Entity.h"
#include "PropertiesPanel.h"

namespace Egl {
	class HierarchyPanel {
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const SceneRef& scene);

		void SetContext(const SceneRef& scene);
		void ResetSelection();
		entt::entity GetSelectedEntity() { return mSelectedEntity; }
		SceneRef GetScene() { return mScene; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(entt::entity e);

		PropertiesPanel mPropertiesPanel;
		entt::entity mSelectedEntity{ entt::null };
		SceneRef mScene;
	};
}