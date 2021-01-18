#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <glm/gtc/type_ptr.hpp>
#include "PropertiesPanel.h"
#include "Eagle/ECS/Components.h"

namespace Egl {
	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::End();

		ImGui::Begin("Properties");
		if (drawedEntity)
			DrawProperties();

		ImGui::End();
	}

	template<typename ComponentType, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity e, UIFunction function) {
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (e.HasComponent<ComponentType>()) {

			auto& component = e.GetComponent<ComponentType>();

			if (ImGui::TreeNodeEx((void*)typeid(ComponentType).hash_code(), flags, name.c_str())) {
				function(component);
				ImGui::TreePop();
			}
		}
	}

	void PropertiesPanel::DrawProperties() {
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
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushFont(boldFont);
		ImGui::Text(drawedEntity.GetComponent<TagComponent>().tag.c_str());
		ImGui::PopFont();
		ImGui::Spacing();

		DrawComponent<TransformComponent>("Transform", drawedEntity, [](auto& component) {
			ImGui::Text("Position  ");

			ImGui::SameLine();
			ImGui::DragFloat3("", glm::value_ptr(component.position), 0.1f);

			ImGui::Text("Rotation  ");
			ImGui::SameLine();
			float rotation = glm::degrees(component.rotation);
			if (ImGui::DragFloat("##rotation", &rotation, 0.5f))
				component.rotation = glm::radians(rotation);

			ImGui::Text("Scale       ");

			ImGui::SameLine();
			ImGui::DragFloat2("##scale", glm::value_ptr(component.scale), 0.1f);
		});

		DrawComponent<CameraComponent>("Camera", drawedEntity, [](auto& component) {
			if (ImGui::Button("Set primary camera")) {

			}

			float size = component.camera.GetCameraSize();
			if (ImGui::DragFloat("Size", &size, 0.05f)) {
				component.camera.SetBounds(size);
			}
			ImGui::ColorEdit4("BG Color", glm::value_ptr(component.backgroundColor));
			ImGui::Checkbox("Fixed Aspectratio", &component.fixedAspectRatio);
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", drawedEntity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});
	}
}