#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/GLM.h>
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
		auto [ tagComp, RelationComp ] = mScene->mRegistry.get<MetadataComponent, Relation>(e);


		ImGuiTreeNodeFlags parentFlags = ((mSelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags leafFlags   = ((mSelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Bullet      | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)e, RelationComp.childCount > 0 ? parentFlags : leafFlags, tagComp.tag.c_str());

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