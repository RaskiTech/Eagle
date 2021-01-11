#include <EaglePCH.h>
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "HierarchyPanel.h"
#include "Eagle/ECS/Components.h"

namespace Egl {
	HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void Egl::HierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		mScene = scene;
	}

	void HierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushFont(boldFont);
		ImGui::Text("Scene Hierarchy");
		ImGui::PopFont();

		mScene->mRegistry.each([&](entt::entity entityID) {
			DrawEntityNode(entityID);
		});

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
			mSelectedEntity = entt::null;
			mPropertiesPanel.SetDrawedEntity({});
		}

		mPropertiesPanel.OnImGuiRender();
	}

	void HierarchyPanel::DrawEntityNode(entt::entity e) {
		TagComponent& tagComp = mScene->mRegistry.get<TagComponent>(e);
		ImGuiTreeNodeFlags flags = ((mSelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)e, flags, tagComp.tag.c_str());

		if (ImGui::IsItemClicked()) {
			mSelectedEntity = e;
			mPropertiesPanel.SetDrawedEntity({ e, mScene.get() });
		}

		if (opened) {
			ImGui::TreePop();
		}
	}
}