#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <glm/gtc/type_ptr.hpp>
#include "HierarchyPanel.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/ComponentsInternal.h"

namespace Egl {
	HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene) {
		SetContext(scene);
	}

	void HierarchyPanel::SetContext(const Ref<Scene>& scene) {
		mScene = scene;
	}
	void HierarchyPanel::ResetSelection() {
		mSelectedEntity = entt::null;
		mPropertiesPanel.SetDrawedEntity({});
	}

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushFont(boldFont);
		ImGui::Text("Scene Hierarchy");
		ImGui::PopFont();

		entt::entity currentEntity = mScene->mFirstEntity;

		while (currentEntity != entt::null) {
			DrawEntityNode(currentEntity);
			currentEntity = mScene->mRegistry.get<Relation>(currentEntity).nextSibling;
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
			mSelectedEntity = entt::null;
			mPropertiesPanel.SetDrawedEntity({});
		}

		mPropertiesPanel.OnImGuiRender();
	}

	void HierarchyPanel::DrawEntityNode(entt::entity e) {
		TagComponent& tagComp = mScene->mRegistry.get<TagComponent>(e);


		ImGuiTreeNodeFlags flags = ((mSelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		// ImGuiTreeNodeFlags_Leaf
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)e, flags, tagComp.tag.c_str());

		if (ImGui::IsItemClicked()) {
			mSelectedEntity = e;
			mPropertiesPanel.SetDrawedEntity({ e, mScene.get() });
		}

		if (opened) {
			auto& comp = mScene->mRegistry.get<Relation>(e);
			auto current = comp.firstChild;
			
			while (current != entt::null) {
				DrawEntityNode(current);
				current = mScene->mRegistry.get<Relation>(current).nextSibling;
			}

			ImGui::TreePop();
		}
	}
}