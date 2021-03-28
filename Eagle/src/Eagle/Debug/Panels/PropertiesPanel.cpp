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

		constexpr float height = 17.5f;

		for (int i = 0; i < optionAmount; i++) {
			ImGui::PushID(UniqueID::GetUniqueFrameID());
			if (i % numInSameLine != 0) ImGui::SameLine();
			else ImGui::Spacing();

			if (ImGui::Selectable(options[i], i == selectedIndex, 0, ImVec2((ImGui::CalcItemWidth() * 1.5f - 30) / numInSameLine, height)))
				selectedIndex = i;

			ImGui::PopID();
		}

		ImGui::Columns(1);

		return selectedIndex;
	}
	template<uint8_t optionAmount>
	static uint8_t SelectWidget(std::array<const char*, optionAmount> options, uint8_t selectedIndex, int numInSameLine = 4) {
		constexpr float height = 17.5f;
		for (int i = 0; i < optionAmount; i++) {
			ImGui::PushID(UniqueID::GetUniqueFrameID());
			if (i % numInSameLine != 0) ImGui::SameLine();
			else ImGui::Spacing();

			if (ImGui::Selectable(options[i], i == selectedIndex, 0, ImVec2((ImGui::CalcItemWidth() * 1.5f - 30) / numInSameLine, height)))
				selectedIndex = i;

			ImGui::PopID();
		}
		return selectedIndex;
	}
	static bool SelectWidget(const char* first, const char* second, bool firstSelected, bool sameLine = true) {
		constexpr float height = 17.5f;

		ImGui::PushID(UniqueID::GetUniqueFrameID());
		if (ImGui::Selectable(first, firstSelected, 0, ImVec2((ImGui::CalcItemWidth() * 1.5f - 30) / 2, height)))
			firstSelected = true;
		ImGui::PopID();
		ImGui::PushID(UniqueID::GetUniqueFrameID());
		ImGui::SameLine();
		if (ImGui::Selectable(second, !firstSelected, 0, ImVec2((ImGui::CalcItemWidth() * 1.5f - 30) / 2, height)))
			firstSelected = false;
		ImGui::PopID();
		return firstSelected;
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

		DrawComponent<UIAlignComponent>("UI Align", drawedEntity, [&](UIAlignComponent& component) {
			uint32_t worldTreeNodeUniqueID = UniqueID::GetUniqueFrameID(); // This because the options use ids so the tree could close when changing transform and sides

			ImGuiIO& io = ImGui::GetIO();
			auto& boldFont = io.Fonts->Fonts[0];

			ImGui::PushFont(boldFont);
			ImGui::Text("Horizontal");
			ImGui::PopFont();
			bool isTransformActive = !component.GetUseSidesHorizontal();
			bool transformActive = SelectWidget("Transform", "Sides", isTransformActive);
			if (transformActive != isTransformActive)
				component.SetUseSidesHorizontal(!transformActive);
			
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 15);
			ImGui::NextColumn();


			if (transformActive) {
				ImGui::Text("Position");
				XDriver xDriver = component.GetXDriver();
				XDriver newXDriver = (XDriver)SelectWidget(std::array<const char*, 5>{"ConstantLeft", "ConstantRight", "RelativeCenter", "RelativeLeft", "RelativeRight"}, (uint8_t)xDriver, 5);
				if (xDriver != newXDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetXDriver(newXDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetXPosValue(newValue);
				}

				float value = component.GetXPosValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value1", &value, value1Speed))
					component.SetXPosValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Scale");
				WidthDriver widthDriver = component.GetWidthDriver();
				WidthDriver newSelectedWidth = (WidthDriver)(SelectWidget(std::array<const char*, 3>{"ConstantWidth", "RelativeWidth", "AspectWidth"}, (uint8_t)widthDriver >> 4, 3) << 4);
				if (widthDriver != newSelectedWidth) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetWidthDriver(newSelectedWidth);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetWidthValue(newValue);
				}

				value = component.GetWidthValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value2", &value, value2Speed))
					component.SetWidthValue(value);
				ImGui::PopItemWidth();
			}
			else {
				ImGui::Text("Left side");
				LeftSideDriver leftDriver = component.GetLeftSideDriver();
				LeftSideDriver newLeftDriver = (LeftSideDriver)SelectWidget(std::array<const char*, 2>{"ConstantOffset", "RelativeOffset"}, (uint8_t)leftDriver, 2);
				if (leftDriver != newLeftDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetLeftSideDriver(newLeftDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetLeftSideValue(newValue);
				}

				float value = component.GetLeftSideValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value3", &value, value1Speed))
					component.SetLeftSideValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Right side");
				RightSideDriver rightDriver = component.GetRightSideDriver();
				RightSideDriver newRightDriver = (RightSideDriver)(SelectWidget(std::array<const char*, 2>{"ConstantOffset", "RelativeOffset"}, (uint8_t)rightDriver >> 4, 2) << 4);
				if (rightDriver != newRightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetRightSideDriver(newRightDriver);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetRightSideValue(newValue);
				}
				value = component.GetRightSideValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value4", &value, value2Speed))
					component.SetRightSideValue(value);
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
			ImGui::Spacing();

			ImGui::PushFont(boldFont);
			ImGui::Text("Vertical");
			ImGui::PopFont();
			isTransformActive = !component.GetUseSidesVertical();
			transformActive = SelectWidget("Transform", "Sides", isTransformActive);
			if (isTransformActive != transformActive)
				component.SetUseSidesVertical(!transformActive);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 15);
			ImGui::NextColumn();

			if (transformActive) {
				ImGui::Text("Position");
				YDriver yDriver = component.GetYDriver();
				YDriver newYDriver = (YDriver)SelectWidget(std::array<const char*, 5>{"ConstantTop", "ConstantBottom", "Center", "RelativeTop", "RelativeBottom"}, (uint8_t)yDriver, 5);
				if (yDriver != newYDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetYDriver(newYDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetYPosValue(newValue);
				}
				float value = component.GetYPosValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value5", &value, value3Speed))
					component.SetYPosValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Scale");
				HeightDriver heightDriver = component.GetHeightDriver();
				HeightDriver newHeightDriver = (HeightDriver)(SelectWidget(std::array<const char*, 3>{"ConstantHeight", "RelativeHeight", "AspectHeight"}, (uint8_t)heightDriver >> 4, 3) << 4);
				if (heightDriver != newHeightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetHeightDriver(newHeightDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetHeightValue(newValue);
				}
				value = component.GetHeightValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value6", &value, value4Speed))
					component.SetHeightValue(value);
				ImGui::PopItemWidth();
			}
			else {
				ImGui::Text("Top");
				TopDriver topDriver = component.GetTopDriver();
				TopDriver newTopDriver = (TopDriver)SelectWidget(std::array<const char*, 2>{"ConstantOffset", "RelativeOffset"}, (uint8_t)topDriver, 2);
				if (topDriver != newTopDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetTopDriver(newTopDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetTopValue(newValue);
				}

				float value = component.GetTopValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value7", &value, value3Speed))
					component.SetTopValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Bottom");
				BottomDriver bottomDriver = component.GetBottomDriver();
				BottomDriver newBottomDriver = (BottomDriver)(SelectWidget(std::array<const char*, 2>{"ConstantOffset", "RelativeOffset"}, (uint8_t)bottomDriver >> 4, 2) << 4);
				if (bottomDriver != newBottomDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetBottomDriver(newBottomDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetBottomValue(newValue);
				}
				value = component.GetBottomValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value8", &value, value4Speed))
					component.SetBottomValue(value);
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
			ImGui::Spacing();

			// World position node
			if (ImGui::TreeNodeEx((void*)(intptr_t)worldTreeNodeUniqueID, ImGuiTreeNodeFlags_SpanAvailWidth, "World Position")) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);
				ImGui::Text("Position");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				ImGui::Text("%.3f  %.3f", component.GetWorldPosition().x, component.GetWorldPosition().y);
				ImGui::PopItemWidth();
				ImGui::Columns(1);

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