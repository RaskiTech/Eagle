#pragma once
#include "Eagle/ECS/Scene.h"
#include "Eagle/Core/Core.h"
#include "Eagle/ECS/Entity.h"
#include "PropertiesPanel.h"

namespace Egl {
	class HierarchyPanel {
	public:
		HierarchyPanel() = default;

		void ResetSelection();
		entt::entity GetSelectedEntity() { return mSelectedEntity; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(entt::entity e, Scene* scene);

		PropertiesPanel mPropertiesPanel;
		entt::entity mSelectedEntity{ entt::null };
	};
}