#pragma once
#include "Eagle/ECS/Scene.h"
#include "Eagle/Core/Core.h"
#include "Eagle/ECS/Entity.h"

namespace Egl {
	class HierarchyPanel {
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(entt::entity e);
		void DrawProperties(Entity e);

		entt::entity mSelectedEntity{ entt::null };
		Ref<Scene> mScene;
	};
}