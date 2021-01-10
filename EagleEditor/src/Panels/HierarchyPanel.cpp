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

		mScene->mRegistry.each([&](entt::entity entityID) {
			DrawEntityNode(entityID);
		});

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
			mSelectedEntity = entt::null;
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (mSelectedEntity != entt::null)
			DrawProperties({ mSelectedEntity, mScene.get() });

		ImGui::End();
	}

	void HierarchyPanel::DrawEntityNode(entt::entity e) {
		TagComponent& tagComp = mScene->mRegistry.get<TagComponent>(e);
		ImGuiTreeNodeFlags flags = ((mSelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)e, flags, tagComp.tag.c_str());

		if (ImGui::IsItemClicked()) {
			mSelectedEntity = e;
		}

		if (opened) {
			ImGui::TreePop();
		}
	}

	void HierarchyPanel::DrawProperties(Entity e) {
		// This gives the oppurtunity to change the name, but there is really no reason, since currently no saving is planned
		//if (e.HasComponent<TagComponent>()) {
		//	auto& tag = e.GetComponent<TagComponent>().tag;
		//	char buffer[256];
		//	memset(buffer, 0, sizeof(buffer));
		//	strcpy(buffer, tag.c_str());
		//	if (ImGui::InputText("tag", buffer, sizeof(buffer))) {
		//		tag = std::string(buffer);
		//	}
		//}
		ImGui::Text(e.GetComponent<TagComponent>().tag.c_str());
		ImGui::Spacing();

		if (e.HasComponent<TransformComponent>()) {

			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = e.GetComponent<TransformComponent>().transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}

		}
	}
}