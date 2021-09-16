#pragma once
#include "Eagle/ECS/Scene.h"
#include "Eagle/Core/Core.h"
#include "Eagle/ECS/Entity.h"
#include "PropertiesPanel.h"

namespace Egl {
	class HierarchyPanel {
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		void ResetSelection();
		entt::entity GetSelectedEntity() { return mSelectedEntity; }
		Ref<Scene> GetScene() { return mScene; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(entt::entity e);

		PropertiesPanel mPropertiesPanel;
		entt::entity mSelectedEntity{ entt::null };
		Ref<Scene> mScene;
	};
}