#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/GLM.h>
#include "Eagle/Core/Application.h"
#include "HierarchyPanel.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/ECS/ComponentsInternal.h"

namespace Egl {
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

		Scene* scene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());

		entt::entity currentEntity = scene->mFirstEntity;

		while (currentEntity != entt::null) {
			DrawEntityNode(currentEntity, scene);
			currentEntity = scene->mRegistry.get<Relation>(currentEntity).nextSibling;
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
			ResetSelection();

		ValidateSelectedEntity();

		mPropertiesPanel.OnImGuiRender();
	}

	void HierarchyPanel::ValidateSelectedEntity() {
		Scene* scene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
		if (mSelectedEntity != entt::null && !scene->mRegistry.valid(mSelectedEntity))
			ResetSelection();
	}

	void HierarchyPanel::DrawEntityNode(entt::entity e, Scene* scene) {
		auto [ tagComp, RelationComp ] = scene->mRegistry.get<MetadataComponent, Relation>(e);

		ImGuiTreeNodeFlags parentFlags = ((mSelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags leafFlags   = ((mSelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Bullet      | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)e, RelationComp.childCount > 0 ? parentFlags : leafFlags, tagComp.tag.c_str());

		if (ImGui::IsItemClicked()) {
			mSelectedEntity = e;
			mPropertiesPanel.SetDrawedEntity({ e, scene });
		}

		if (opened) {
			auto& comp = scene->mRegistry.get<Relation>(e);
			auto current = comp.firstChild;
			
			while (current != entt::null) {
				DrawEntityNode(current, scene);
				current = scene->mRegistry.get<Relation>(current).nextSibling;
			}

			ImGui::TreePop();
		}
	}
}