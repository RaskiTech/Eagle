#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/ImGuiInternal.h>
#include <glm/gtc/type_ptr.hpp>
#include "PropertiesPanel.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/Core/UniqueID.h"
#include "Eagle/Core/Application.h"

namespace Egl {
	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::End();

		ImGui::Begin("Properties");
		if (drawedEntity)
			DrawProperties();

		ImGui::End();
	}

	#pragma region Custom Widgets

	template<typename ComponentType, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity e, UIFunction function) {
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (e.HasComponent<ComponentType>()) {
			
			auto& component = e.GetComponent<ComponentType>();

			if (ImGui::TreeNodeEx((void*)(intptr_t)UniqueID::GetUniqueFrameID(), flags, name.c_str())) {
				function(component);
				ImGui::TreePop();
			}
		}
	}

	static bool DrawVec2(const char* name, float* value_ptr) {
		ImGui::PushID(UniqueID::GetUniqueFrameID());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, 100);
		ImGui::Text(name);
		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
		bool isTrue = ImGui::DragFloat2("##Transform", value_ptr, 0.1f, 0, 0, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopID();
		return isTrue;
	}
	static bool DrawFloat(const char* name, float* value) {
		ImGui::PushID(UniqueID::GetUniqueFrameID());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, 100);
		ImGui::Text(name);
		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
		bool isTrue = ImGui::DragFloat("##Transform", value, 0.1f, 0, 0, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopID();
		return isTrue;
	}

	template<uint8_t optionAmount>
	static uint8_t SelectWidget(const char* name, std::array<const char*, optionAmount> options, uint8_t selectedIndex, int numInSameLine = 4) {
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, 100);
		ImGui::Text(name);
		ImGui::NextColumn();

		constexpr float height = 20;

		for (int i = 0; i < optionAmount; i++) {
			ImGui::PushID(UniqueID::GetUniqueFrameID());
			if (i % numInSameLine != 0) ImGui::SameLine();

			if (ImGui::Selectable(options[i], i == selectedIndex, 0, ImVec2((ImGui::CalcItemWidth() * 1.5f - 30) / numInSameLine, height)))
				selectedIndex = i;

			ImGui::PopID();
		}

		ImGui::Columns(1);

		return selectedIndex;
	}

	#pragma endregion

	void PropertiesPanel::DrawProperties() {
		// This gives the oppurtunity to change the name, but there is really no reason, since saving isn't in development yet
		//if (e.HasComponent<MetadataComponent>()) {
		//	auto& tag = e.GetComponent<MetadataComponent>().tag;
		//	char buffer[256];
		//	memset(buffer, 0, sizeof(buffer));
		//	strcpy(buffer, tag.c_str());
		//	if (ImGui::InputText("tag", buffer, sizeof(buffer))) {
		//		tag = std::string(buffer);
		//	}
		//}

		ImGuiIO& io = ImGui::GetIO();
		auto& metaComponent = drawedEntity.GetComponent<MetadataComponent>();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushFont(boldFont);
		ImGui::Text(metaComponent.tag.c_str());
		ImGui::PopFont();

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text("Layer");
		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.745f);
		int sorting = metaComponent.sortingLayer;
		if (ImGui::DragInt("##mainL", &sorting, 1, 0, 127))
			metaComponent.sortingLayer = sorting;
		ImGui::SameLine();
		sorting = metaComponent.subSorting;
		if (ImGui::DragInt("##subL", &sorting, 1, 0, 127))
			metaComponent.subSorting = sorting;
		ImGui::Columns(1);
		ImGui::PopItemWidth();
		ImGui::Spacing();


		DrawComponent<TransformComponent>("Transform", drawedEntity, [](TransformComponent& component) {
			auto pos = component.GetLocalPosition();
			if (DrawVec2("Position", glm::value_ptr(pos)))
				component.SetLocalPosition(pos);

			auto rot = component.GetLocalRotation();
			if (DrawFloat("Rotation", &rot))
				component.SetLocalRotation(rot);

			auto scale = component.GetLocalScale();
			if (DrawVec2("Scale", glm::value_ptr(scale)))
				component.SetLocalScale(scale);
		});

		DrawComponent<UIAlignComponent>("UIAlign", drawedEntity, [](UIAlignComponent& component) {
			component.xDriver = (UIAlignComponent::XDriver)SelectWidget("X Driver", std::array<const char*, 5>{"PixelsFromLeft", "PixelsFromRight", "AlignCenterX", "AlignLeft", "AlignRight"}, (uint8_t)component.xDriver, 5);
			DrawFloat("X Position", &component.xPosValue);
			ImGui::Spacing();

			component.yDriver = (UIAlignComponent::YDriver)SelectWidget("Width Driver", std::array<const char*, 5>{"PixelsFromTop", "PixelsFromBottom", "AlignCenterY", "AlignTop", "AlignBottom"}, (uint8_t)component.yDriver, 5);
			DrawFloat("Y Position", &component.yPosValue);
			ImGui::Spacing();

			component.widthDriver = (UIAlignComponent::WidthDriver)SelectWidget("Width Driver", std::array<const char*, 3>{"ConstantWidth", "RelativeWidth", "AspectWidth"}, (uint8_t)component.widthDriver, 3);
			DrawFloat("Width Value", &component.widthValue);
			ImGui::Spacing();

			component.heightDriver = (UIAlignComponent::HeightDriver)SelectWidget("Width Driver", std::array<const char*, 3>{"ConstantHeight", "RelativeHeight", "AspectHeight"}, (uint8_t)component.heightDriver, 3);
			DrawFloat("Height Value", &component.heightValue);
			ImGui::Spacing();

			if (ImGui::TreeNodeEx((void*)(intptr_t)UniqueID::GetUniqueFrameID(), 0, "World Position")) {

				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, 100);
				ImGui::Text("Position");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				ImGui::Text("%.3f  %.3f", component.GetWorldPosition().x, component.GetWorldPosition().y);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				// ImGui::Columns(2);
				// 
				// ImGui::SetColumnWidth(0, 100);
				// ImGui::Text("Rotation");
				// ImGui::NextColumn();
				// 
				// ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				// float rotation = component.GetLocalRotation();
				// if (ImGui::DragFloat("##Rotation", &rotation, 0.1f, 0, 0, "%.2f"))
				// 	component.SetLocalRotation(rotation);
				// ImGui::PopItemWidth();
				// 
				// ImGui::Columns(1);
				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, 100);
				ImGui::Text("Scale");
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				ImGui::Text("%.3f  %.3f", component.GetWorldScale().x, component.GetWorldScale().y);
				ImGui::PopItemWidth();

				ImGui::Columns(1);
				ImGui::TreePop();
			}
		});

		DrawComponent<CameraComponent>("Camera", drawedEntity, [&](CameraComponent& component) {
			if (ImGui::Button("Set primary camera")) {
				auto scene = Application::Get().GetGameLayer()->GetActiveScene();
				scene->SetPrimaryCamera(drawedEntity);
			}

			float size = component.camera.GetSize();
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
			ImGui::Text("Max particles: %d", component.particleSystem.AllParticlesCount());
			ImGui::Text("Alive particles: %d", component.particleSystem.AliveParticlesCount());
			ImGui::Spacing();

			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
			const ImGuiTreeNodeFlags flagsNotOpen = ImGuiTreeNodeFlags_SpanAvailWidth;

			if (ImGui::TreeNodeEx((void*)(intptr_t)UniqueID::GetUniqueFrameID(), flags, "Emitters")) {
				auto& emitters = component.particleSystem.GetEmitters();
				for (auto& emitter : emitters) {
					float emitPerSec = emitter->GetEmistPerSecond();
					ImGui::Text("Emitting amount per second");
					if (ImGui::DragFloat("", &emitPerSec, emitPerSec / 500))
						emitter->SetEmitsPerSecond(emitPerSec);
					ImGui::Spacing();
					auto& generators = emitter->GetSetters();

					if (ImGui::TreeNodeEx((void*)(intptr_t)UniqueID::GetUniqueFrameID(), flagsNotOpen, "Generators")) {
						for(auto& generator : generators)
							generator->OnImGuiRender();
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx((void*)(intptr_t)UniqueID::GetUniqueFrameID(), flags, "Updaters")) {
				auto& updaters = component.particleSystem.GetUpdaters();
				for (auto& updater : updaters)
					updater->OnImGuiRender();
				ImGui::TreePop();
			}
		});
	}
}