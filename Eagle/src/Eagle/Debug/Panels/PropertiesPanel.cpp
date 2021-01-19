#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <glm/gtc/type_ptr.hpp>
#include "PropertiesPanel.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/Core/UniqueID.h"
#include <imgui_internal.h>

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

			if (ImGui::TreeNodeEx((void*)UniqueID::GetUniqueFrameID(), flags, name.c_str())) {
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

		DrawComponent<TransformComponent>("Transform", drawedEntity, [](TransformComponent& component) {
			ImGui::PushID(UniqueID::GetUniqueFrameID());
			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, 100);
			ImGui::Text("Position");
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::CalcItemWidth()*1.5f);
			ImGui::DragFloat3("##Transform", glm::value_ptr(component.position), 0.1f, 0, 0, "%.2f");
			ImGui::PopItemWidth();

			ImGui::Columns(1);
			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, 100);
			ImGui::Text("Rotation");
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
			ImGui::DragFloat("##Rotation", &component.rotation, 0.1f, 0, 0, "%.2f");
			ImGui::PopItemWidth();

			ImGui::Columns(1);
			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, 100);
			ImGui::Text("Scale");
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
			ImGui::DragFloat2("##Scale", glm::value_ptr(component.scale), 0.1f, 0, 0, "%.2f");
			ImGui::PopItemWidth();

			ImGui::Columns(1);
			ImGui::PopID();
		});

		DrawComponent<CameraComponent>("Camera", drawedEntity, [](CameraComponent& component) {
			if (ImGui::Button("Set primary camera")) {

			}

			float size = component.camera.GetCameraSize();
			if (ImGui::DragFloat("Size", &size, 0.05f)) {
				component.camera.SetBounds(size);
			}
			ImGui::ColorEdit4("BG Color", glm::value_ptr(component.backgroundColor));
			ImGui::Checkbox("Fixed Aspectratio", &component.fixedAspectRatio);
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", drawedEntity, [](SpriteRendererComponent& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});

		DrawComponent<ParticleSystemComponent>("Particle System", drawedEntity, [](ParticleSystemComponent& component) {
			ImGuiIO& io = ImGui::GetIO();
			auto& boldFont = io.Fonts->Fonts[0];
			ImGui::Text("Max particles: %d", component.particleSystem.AllParticlesCount());
			ImGui::Text("Alive particles: %d", component.particleSystem.AliveParticlesCount());
			ImGui::Spacing();

			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
			const ImGuiTreeNodeFlags flagsNotOpen = ImGuiTreeNodeFlags_SpanAvailWidth;

			if (ImGui::TreeNodeEx((void*)UniqueID::GetUniqueFrameID(), flags, "Emitters")) {
				auto& emitters = component.particleSystem.GetEmitters();
				for (auto& emitter : emitters) {

					ImGui::DragFloat("Emitter emit amount", &emitter->mEmitsPerSecond, emitter->mEmitsPerSecond / 500);
					ImGui::Spacing();
					auto& generators = emitter->GetGenerators();

					if (ImGui::TreeNodeEx((void*)UniqueID::GetUniqueFrameID(), flagsNotOpen, "Generators")) {
						for(auto& generator : generators)
							generator->OnImGuiRender();
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx((void*)UniqueID::GetUniqueFrameID(), flags, "Updaters")) {
				auto& updaters = component.particleSystem.GetUpdaters();
				for (auto& updater : updaters)
					updater->OnImGuiRender();
				ImGui::TreePop();
			}
		});
	}
}