#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/ImGuiInternal.h>
#include <Dependencies/GLM.h>
#include "PropertiesPanel.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/Core/UniqueID.h"
#include "Eagle/Core/Application.h"

#define SLIDER_FRICTION 512

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
	static inline void ComponentEditorInit(Entity e, UIFunction func) {
		if (e.HasComponent<ComponentType>())
			func(e.GetComponent<ComponentType>());
	}
	template<typename ComponentType, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity e, UIFunction function) {
		constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (e.HasComponent<ComponentType>()) {
			
			auto& component = e.GetComponent<ComponentType>();

			if (ImGui::TreeNodeEx((void*)(intptr_t)UniqueID::GetUniqueFrameID(), flags, name.c_str())) {
				ImGui::TreePop();
				function(component);
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

	void PropertiesPanel::SetDrawedEntity(Entity e) { 
		drawedEntity = e;

		if (e.IsValid()) // Check that the entity isn't null
			ComponentEditorInit<UITransform>(e, [&](UITransform& component) {
				float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
				value1Speed = camSize * (component.GetPrimaryXFromWorldPos(1) - component.GetPrimaryXFromWorldPos(0)) / SLIDER_FRICTION;
				value2Speed = camSize * (component.GetSecondaryXFromWorldScale(1) - component.GetSecondaryXFromWorldScale(0)) / SLIDER_FRICTION;
				value3Speed = camSize * (component.GetPrimaryXFromWorldPos(1) - component.GetPrimaryXFromWorldPos(0)) / SLIDER_FRICTION;
				value4Speed = camSize * (component.GetSecondaryXFromWorldScale(1) - component.GetSecondaryXFromWorldScale(0)) / SLIDER_FRICTION;
			});
	}

	void PropertiesPanel::DrawProperties() {
		// This gives the oppurtunity to change the name, but there is really no reason for it
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


		DrawComponent<Transform>("Transform", drawedEntity, [](Transform& component) {
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

		DrawComponent<UITransform>("UI Transform", drawedEntity, [&](UITransform& component) {
			uint32_t worldTreeNodeUniqueID = UniqueID::GetUniqueFrameID(); // This because the options use ids so the tree could close when changing transform and sides

			ImGuiIO& io = ImGui::GetIO();
			auto& boldFont = io.Fonts->Fonts[0];

			ImGui::Columns(1);

			ImGui::PushFont(boldFont);
			ImGui::Text("Horizontal");
			ImGui::PopFont();
			bool isTransformActive = !component.GetUseSidesHorizontal();
			bool transformActive = SelectWidget("Transform", "Sides", isTransformActive);
			if (transformActive != isTransformActive) {
				const glm::vec2& worldPos = component.GetWorldPosition();
				const glm::vec2& worldScale = component.GetWorldScale();

				component.SetUseSidesHorizontal(!transformActive);
				float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();

				// Set slider speeds and values, so UI isn't awful to design with
				value1Speed = camSize * (component.GetPrimaryXFromWorldPos(1) - component.GetPrimaryXFromWorldPos(0)) / SLIDER_FRICTION;
				value2Speed = camSize * (component.GetSecondaryXFromWorldScale(1) - component.GetSecondaryXFromWorldScale(0)) / SLIDER_FRICTION;

				float newPrim = component.GetPrimaryXFromWorldPos(worldPos.x);
				component.SetXPosValue(newPrim);
				float newSec = component.GetSecondaryXFromWorldScale(worldScale.x);
				component.SetWidthValue(newSec);
			}
			
			ImGui::Columns(2, "id_Hor");
			ImGui::SetColumnWidth(0, 15);
			ImGui::NextColumn();


			if (transformActive) {
				ImGui::Text("Position");
				UITransform::XDriver xDriver = component.GetXDriver();
				UITransform::XDriver newXDriver = (UITransform::XDriver)
					SelectWidget(std::array<const char*, 5>{"ConstLeft", "ConstRight", "RelativeCenter", "RelativeLeft", "RelativeRight"}, (uint8_t)xDriver, 5);
				if (xDriver != newXDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetXDriver(newXDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetXPosValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value1Speed = camSize*(component.GetPrimaryXFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}

				float value = component.GetXPosValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value1", &value, value1Speed))
					component.SetXPosValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Scale");
				UITransform::WidthDriver widthDriver = component.GetWidthDriver();
				UITransform::WidthDriver newSelectedWidth = (UITransform::WidthDriver)
					(SelectWidget(std::array<const char*, 3>{"ConstWidth", "RelativeWidth", "AspectWidth"}, (uint8_t)widthDriver >> 4, 3) << 4);
				if (widthDriver != newSelectedWidth) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetWidthDriver(newSelectedWidth);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetWidthValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value2Speed = camSize * (component.GetSecondaryXFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
				}

				value = component.GetWidthValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value2", &value, value2Speed))
					component.SetWidthValue(value);
				ImGui::PopItemWidth();
			}
			else {
				ImGui::Text("Left side");
				UITransform::LeftDriver leftDriver = component.GetLeftSideDriver();
				UITransform::LeftDriver newLeftDriver = (UITransform::LeftDriver)
					SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)leftDriver, 2);
				if (leftDriver != newLeftDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetLeftSideDriver(newLeftDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetLeftSideValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value1Speed = camSize * (component.GetPrimaryXFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}

				float value = component.GetLeftSideValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value3", &value, value1Speed))
					component.SetLeftSideValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Right side");
				UITransform::RightDriver rightDriver = component.GetRightSideDriver();
				UITransform::RightDriver newRightDriver = (UITransform::RightDriver)
					(SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)rightDriver >> 4, 2) << 4);
				if (rightDriver != newRightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetRightSideDriver(newRightDriver);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetRightSideValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value2Speed = camSize * (component.GetSecondaryXFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
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
			if (isTransformActive != transformActive) {
				const glm::vec2& worldPos = component.GetWorldPosition();
				const glm::vec2& worldScale = component.GetWorldScale();

				component.SetUseSidesVertical(!transformActive);
				float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();

				// Set slider speeds and values, so UI isn't awful to design
				value3Speed = camSize * (component.GetPrimaryYFromWorldPos(1) - component.GetPrimaryYFromWorldPos(0)) / SLIDER_FRICTION;
				value4Speed = camSize * (component.GetSecondaryYFromWorldScale(1) - component.GetSecondaryYFromWorldScale(0)) / SLIDER_FRICTION;

				float newPrim = component.GetPrimaryYFromWorldPos(worldPos.y);
				component.SetYPosValue(newPrim);
				float newSec = component.GetSecondaryYFromWorldScale(worldScale.y);
				component.SetHeightValue(newSec);
			}

			ImGui::Columns(2, "id_Ver");
			ImGui::SetColumnWidth(0, 15);
			ImGui::NextColumn();

			if (transformActive) {
				ImGui::Text("Position");
				UITransform::YDriver yDriver = component.GetYDriver();
				UITransform::YDriver newYDriver = (UITransform::YDriver)SelectWidget(std::array<const char*, 5>{"ConstTop", "ConstBottom", "Center", "RelativeTop", "RelativeBottom"}, (uint8_t)yDriver, 5);
				if (yDriver != newYDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetYDriver(newYDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetYPosValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value3Speed = camSize * (component.GetPrimaryYFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}
				float value = component.GetYPosValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value5", &value, value3Speed))
					component.SetYPosValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Scale");
				UITransform::HeightDriver heightDriver = component.GetHeightDriver();
				UITransform::HeightDriver newHeightDriver = (UITransform::HeightDriver)(SelectWidget(std::array<const char*, 3>{"ConstHeight", "RelativeHeight", "AspectHeight"}, (uint8_t)heightDriver >> 4, 3) << 4);
				if (heightDriver != newHeightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetHeightDriver(newHeightDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetHeightValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value4Speed = camSize * (component.GetSecondaryYFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
				}
				value = component.GetHeightValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value6", &value, value4Speed))
					component.SetHeightValue(value);
				ImGui::PopItemWidth();
			}
			else {
				ImGui::Text("Top");
				UITransform::TopDriver topDriver = component.GetTopDriver();
				UITransform::TopDriver newTopDriver = (UITransform::TopDriver)SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)topDriver, 2);
				if (topDriver != newTopDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetTopDriver(newTopDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetTopValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value3Speed = camSize * (component.GetPrimaryYFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}

				float value = component.GetTopValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value7", &value, value3Speed))
					component.SetTopValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::Text("Bottom");
				UITransform::BottomDriver bottomDriver = component.GetBottomDriver();
				UITransform::BottomDriver newBottomDriver = (UITransform::BottomDriver)(SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)bottomDriver >> 4, 2) << 4);
				if (bottomDriver != newBottomDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetBottomDriver(newBottomDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetBottomValue(newValue);
					float camSize = Application::Get().GetGameLayer()->GetActiveScene()->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					value4Speed = camSize * (component.GetSecondaryYFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
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

		DrawComponent<TextComponent>("Text component", drawedEntity, [](TextComponent& comp) {
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, comp.GetText().c_str());
			if (ImGui::InputTextMultiline("Text", buffer, sizeof(buffer), ImVec2(0, 80))) {
				comp.SetText(std::string(buffer));
			}
			comp.data.alignHorizontal = (TextAlignHorizontal)SelectWidget("Horizontal", std::array<const char*, 3>{"Left", "Middle", "Right"}, (uint8_t)comp.data.alignHorizontal);
			comp.data.alignVertical = (TextAlignVertical)SelectWidget("Vertical", std::array<const char*, 3>{"Top", "Middle", "Bottom"}, (uint8_t)comp.data.alignVertical);

			ImGui::DragFloat("Font size", &comp.data.fontSize, 0.02f, 0, 100);

			ImGui::ColorEdit4("Color", glm::value_ptr(comp.data.color));
			ImGui::DragInt("Chars visible", (int*)&comp.data.charsVisible, 1, -1);
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", drawedEntity, [](SpriteRendererComponent& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
			if (component.texture != -1) ImGui::Text("Has texture: True");
			else ImGui::Text("Has texture: False");
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

		DrawComponent<AudioSource>("Audio Source", drawedEntity, [](AudioSource& source) {
			ImGui::Text("Playing: %s", source.IsPlaying() ? "True" : "False");
			ImGui::Text("Length: %.1f seconds", source.GetDuration());
			ImGui::Spacing();

			if (ImGui::Button(source.IsPlaying() ? "Pause" : "Play")) {
				source.Play(!source.IsPlaying());
			}
			if (ImGui::Button(source.GetIsLooping() ? "Looping" : "Not looping"))
				source.SetIsLooping(!source.GetIsLooping());
			
			float time = source.GetTime();
			if (ImGui::SliderFloat("Time", &time, 0, source.GetDuration()))
				source.SetTime(time);

			float vol = source.GetVolume();
			if (ImGui::SliderFloat("Volume", &vol, 0, 1))
				source.SetVolume(vol);

		});
	}
}