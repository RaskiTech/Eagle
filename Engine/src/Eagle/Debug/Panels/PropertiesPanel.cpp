#include <EaglePCH.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/ImGuiInternal.h>
#include <Dependencies/GLM.h>
#include "PropertiesPanel.h"
#include "Eagle/ECS/Components.h"
#include "Eagle/Core/Application.h"
#include "Eagle/Debug/ImGui/ImGuiLayer.h"

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

			ImGui::PushStyleColor(ImGuiCol_Header,        EDITOR_COLOR_VIOLET1);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_VIOLET1);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive,  EDITOR_COLOR_VIOLET1);

			bool render = ImGui::TreeNodeEx(name.c_str(), flags, name.c_str());

			ImGui::PopStyleColor(3);

			if (render)
			{
				ImGui::TreePop();
				function(component);
			}
		}
	}

	static bool DrawVec2(const char* name, float* value_ptr, const char* id) {
		ImGui::PushID(id);
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
	static bool DrawFloat(const char* name, float* value, const char* id) {
		ImGui::PushID(id);
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

	void CenteredTextInColumn(const char* text) {
		float columnWidth = ImGui::GetColumnWidth(ImGui::GetColumnIndex());
		float textWidth   = ImGui::CalcTextSize(text).x;

		float startBuffer = 0;
		for (int i = 0; i < ImGui::GetColumnIndex(); i++)
			startBuffer += ImGui::GetColumnWidth(i);

		ImGui::SetCursorPosX(startBuffer + (columnWidth - textWidth) * 0.5f);
		ImGui::Text(text);
	}
	void SelectableBGColor(ImU32 color) {
		ImVec2 p_min = ImGui::GetItemRectMin();
		ImVec2 p_max = ImGui::GetItemRectMax();
		ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, color);
	}

	template<uint8_t optionAmount>
	static uint8_t SelectWidget(const char* name, std::array<const char*, optionAmount> options, uint8_t selectedIndex, int numInSameLine, const std::string& id, int rightSideOffset = 0) {
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, 100);
		ImGui::Text(name);
		ImGui::NextColumn();

		constexpr float height = 17.5f;
		float oneButtonWidth = (ImGui::GetColumnWidth() - rightSideOffset) / numInSameLine;

		std::string uniqueID = id + '0';
		for (int i = 0; i < optionAmount; i++) {
			uniqueID[uniqueID.length() - 1] = i;

			ImGui::PushID(uniqueID.c_str());
			if (i % numInSameLine != 0) ImGui::SameLine();
			else ImGui::Spacing();

			if (ImGui::Selectable(options[i], i == selectedIndex, 0, ImVec2(oneButtonWidth, height)))
				selectedIndex = i;

			ImGui::PopID();
		}

		ImGui::Columns(1);

		return selectedIndex;
	}
	template<uint8_t optionAmount>
	static uint8_t SelectWidget(std::array<const char*, optionAmount> options, uint8_t selectedIndex, int numInSameLine, const std::string& id, int rightSideOffset = 0) {
		constexpr float height = 17.5f;
		float oneButtonWidth = (ImGui::GetContentRegionAvailWidth() - rightSideOffset) / numInSameLine;
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		

		std::string uniqueID = id + '0';
		for (int i = 0; i < optionAmount; i++) {
			uniqueID[uniqueID.length() - 1] = i;

			//ImGui::PushID(uniqueID.c_str());
			if (i % numInSameLine != 0) ImGui::SameLine();
			else                        ImGui::Spacing();

			// Render background to selectable if it is not hovered. //

			// Channel number is like z-order. Widgets in higher channels are rendered above widgets in lower channels.
			draw_list->ChannelsSplit(2);
			draw_list->ChannelsSetCurrent(1);
			if (ImGui::Selectable(options[i], i == selectedIndex, ImGuiSelectableFlags_SelectOnClick, ImVec2(oneButtonWidth, height)))
				selectedIndex = i;
			if (!ImGui::IsItemHovered() && i != selectedIndex) {
				draw_list->ChannelsSetCurrent(0);
				ImVec4 col = EDITOR_COLOR_DARKGRAY3;
				SelectableBGColor(IM_COL32(col.x * 255, col.y * 255, col.z * 255, col.w * 255));
			}
			draw_list->ChannelsMerge();

		}
		return selectedIndex;
	}
	static bool SelectWidget(const char* first, const char* second, bool firstSelected, bool sameLine, const std::string& id) {
		constexpr float height = 17.5f;

		std::string uniqueID = id + '0';
		ImGui::PushID(uniqueID.c_str());
		if (ImGui::Selectable(first, firstSelected, 0, ImVec2((ImGui::CalcItemWidth() * 1.5f - 30) / 2, height)))
			firstSelected = true;
		ImGui::PopID();
		uniqueID[uniqueID.length() - 1] = '1';
		ImGui::PushID(uniqueID.c_str());
		ImGui::SameLine();
		if (ImGui::Selectable(second, !firstSelected, 0, ImVec2((ImGui::CalcItemWidth() * 1.5f - 30) / 2, height)))
			firstSelected = false;
		ImGui::PopID();
		return firstSelected;
	}
	template<uint8_t optionAmount>
	static uint8_t ToggleWidget(const std::string& name, std::array<const char*, optionAmount> options, uint8_t selectedIndex) {
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text(name.c_str());

		ImGui::NextColumn();
		
		if (ImGui::Button(options[selectedIndex], ImVec2(ImGui::GetColumnWidth(), 0)))
			selectedIndex = (selectedIndex + 1) % optionAmount;

		ImGui::Columns(1);
		ImGui::PopID();
		return selectedIndex;
	}

	#pragma endregion

	void PropertiesPanel::SetDrawedEntity(Entity e) { 
		drawedEntity = e;

		if (e.IsValid()) // Check that the entity isn't null
			ComponentEditorInit<UITransform>(e, [&](UITransform& component) {
				float camSize = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene())->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
				_value1Speed = camSize * (component.GetPrimaryXFromWorldPos(1) - component.GetPrimaryXFromWorldPos(0)) / SLIDER_FRICTION;
				_value2Speed = camSize * (component.GetSecondaryXFromWorldScale(1) - component.GetSecondaryXFromWorldScale(0)) / SLIDER_FRICTION;
				_value3Speed = camSize * (component.GetPrimaryXFromWorldPos(1) - component.GetPrimaryXFromWorldPos(0)) / SLIDER_FRICTION;
				_value4Speed = camSize * (component.GetSecondaryXFromWorldScale(1) - component.GetSecondaryXFromWorldScale(0)) / SLIDER_FRICTION;
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
			if (DrawVec2("Position", glm::value_ptr(pos), "#pos"))
				component.SetLocalPosition(pos);

			auto rot = component.GetLocalRotation();
			if (DrawFloat("Rotation", &rot, "#rot"))
				component.SetLocalRotation(rot);

			auto scale = component.GetLocalScale();
			if (DrawVec2("Scale", glm::value_ptr(scale), "#scl"))
				component.SetLocalScale(scale);
		});

		DrawComponent<UITransform>("UI Transform", drawedEntity, [&](UITransform& component) {
			Scene* scene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());

			ImGuiIO& io = ImGui::GetIO();
			auto& boldFont = io.Fonts->Fonts[0];

			bool hUseSides = ToggleWidget<2>("Horizontal", std::array<const char*, 2>{"Transform", "Sides"}, (uint8_t)component.GetUseSidesHorizontal());
			bool vUseSides = ToggleWidget<2>("Vertical", std::array<const char*, 2>{"Transform", "Sides"}, (uint8_t)component.GetUseSidesVertical());

			// Automatically convert values to the new system, so when switching between 
			// transform and sides, the object doesn't jump all over the place
			if (hUseSides != component.GetUseSidesHorizontal()) {
				const glm::vec2& worldPos = component.GetWorldPosition();
				const glm::vec2& worldScale = component.GetWorldScale();

				component.SetUseSidesHorizontal(hUseSides);
				float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();

				// Set slider speeds and values, so UI isn't awful to design with
				_value1Speed = camSize * (component.GetPrimaryXFromWorldPos(1) - component.GetPrimaryXFromWorldPos(0)) / SLIDER_FRICTION;
				_value2Speed = camSize * (component.GetSecondaryXFromWorldScale(1) - component.GetSecondaryXFromWorldScale(0)) / SLIDER_FRICTION;

				float newPrim = component.GetPrimaryXFromWorldPos(worldPos.x);
				component.SetXPosValue(newPrim);
				float newSec = component.GetSecondaryXFromWorldScale(worldScale.x);
				component.SetWidthValue(newSec);

			}
			if (vUseSides != component.GetUseSidesVertical()) {
				const glm::vec2& worldPos = component.GetWorldPosition();
				const glm::vec2& worldScale = component.GetWorldScale();

				component.SetUseSidesVertical(vUseSides);
				float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();

				// Set slider speeds and values, so UI isn't awful to design
				_value3Speed = camSize * (component.GetPrimaryYFromWorldPos(1) - component.GetPrimaryYFromWorldPos(0)) / SLIDER_FRICTION;
				_value4Speed = camSize * (component.GetSecondaryYFromWorldScale(1) - component.GetSecondaryYFromWorldScale(0)) / SLIDER_FRICTION;

				float newPrim = component.GetPrimaryYFromWorldPos(worldPos.y);
				component.SetYPosValue(newPrim);
				float newSec = component.GetSecondaryYFromWorldScale(worldScale.y);
				component.SetHeightValue(newSec);
			}

			ImGui::Spacing();
			ImGui::Spacing();

			if (!hUseSides) {
				ImGui::PushFont(boldFont);
				ImGui::Text("Horizontal Position");
				ImGui::PopFont();

				ImGui::Columns(2, "##constantRelativeColumn");
				float nextSelectableOneButtonWidth = ImGui::GetWindowContentRegionWidth() / 5;
				ImGui::SetColumnWidth(0, nextSelectableOneButtonWidth * 2);
				CenteredTextInColumn("Constant");
				ImGui::NextColumn();

				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 vec = ImGui::GetCursorScreenPos();
				ImVec4 col = EDITOR_COLOR_DARKGRAY3;
				ImU32 lineColor = IM_COL32(col.x * 255, col.y * 255, col.z * 255, col.w * 255);
				drawList->AddLine(ImVec2(vec.x - 5, vec.y + ImGui::GetTextLineHeightWithSpacing()), ImVec2(vec.x - 5, vec.y), lineColor, 5);

				CenteredTextInColumn("Relative");
				ImGui::Columns(1);

				UITransform::XDriver old_xDriver = component.GetXDriver();
				UITransform::XDriver new_xDriver = (UITransform::XDriver)
					SelectWidget(std::array<const char*, 5>{"Left", "Right", "Center", "Left", "Right"}, (uint8_t)old_xDriver, 5, "#position100", 30);
				
				if (old_xDriver != new_xDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetXDriver(new_xDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetXPosValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value1Speed = camSize*(component.GetPrimaryXFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}

				float value = component.GetXPosValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value1", &value, _value1Speed))
					component.SetXPosValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::PushFont(boldFont);
				ImGui::Text("Width");
				ImGui::PopFont();
				UITransform::WidthDriver widthDriver = component.GetWidthDriver();
				UITransform::WidthDriver newSelectedWidth = (UITransform::WidthDriver)
					(SelectWidget(std::array<const char*, 3>{"Constant", "Relative", "Aspect"}, (uint8_t)widthDriver >> 4, 3, "#scale100") << 4);
				if (widthDriver != newSelectedWidth) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetWidthDriver(newSelectedWidth);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetWidthValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value2Speed = camSize * (component.GetSecondaryXFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
				}

				value = component.GetWidthValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value2", &value, _value2Speed))
					component.SetWidthValue(value);
				ImGui::PopItemWidth();
			}
			else {
				ImGui::PushFont(boldFont);
				ImGui::Text("Left side");
				ImGui::PopFont();
				UITransform::LeftDriver leftDriver = component.GetLeftSideDriver();
				UITransform::LeftDriver newLeftDriver = (UITransform::LeftDriver)
					SelectWidget(std::array<const char*, 2>{"Constant", "Relative"}, (uint8_t)leftDriver, 2, "#leftSide100");
				if (leftDriver != newLeftDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetLeftSideDriver(newLeftDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetLeftSideValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value1Speed = camSize * (component.GetPrimaryXFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}

				float value = component.GetLeftSideValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value3", &value, _value1Speed))
					component.SetLeftSideValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::PushFont(boldFont);
				ImGui::Text("Right side");
				ImGui::PopFont();
				UITransform::RightDriver rightDriver = component.GetRightSideDriver();
				UITransform::RightDriver newRightDriver = (UITransform::RightDriver)
					(SelectWidget(std::array<const char*, 2>{"Constant", "Relative"}, (uint8_t)rightDriver >> 4, 2, "#rightside") << 4);
				if (rightDriver != newRightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetRightSideDriver(newRightDriver);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetRightSideValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value2Speed = camSize * (component.GetSecondaryXFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
				}
				value = component.GetRightSideValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value4", &value, _value2Speed))
					component.SetRightSideValue(value);
				ImGui::PopItemWidth();
			}

			ImGui::Spacing();
			ImGui::Spacing();

			if (!vUseSides) {
				ImGui::PushFont(boldFont);
				ImGui::Text("Vertical Position");
				ImGui::PopFont();

				ImGui::Columns(2, "#constantRelativeColumn21");
				float nextSelectableOneButtonWidth = ImGui::GetWindowContentRegionWidth() / 5;
				ImGui::SetColumnWidth(0, nextSelectableOneButtonWidth * 2);
				CenteredTextInColumn("Constant");
				ImGui::NextColumn();

				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 vec = ImGui::GetCursorScreenPos();
				ImVec4 col = EDITOR_COLOR_DARKGRAY3;
				ImU32 lineColor = IM_COL32(col.x * 255, col.y * 255, col.z * 255, col.w * 255);
				drawList->AddLine(ImVec2(vec.x - 5, vec.y + ImGui::GetTextLineHeightWithSpacing()), ImVec2(vec.x - 5, vec.y), lineColor, 5);

				CenteredTextInColumn("Relative");
				ImGui::Columns(1);

				UITransform::YDriver old_yDriver = component.GetYDriver();
				UITransform::YDriver new_yDriver = (UITransform::YDriver)SelectWidget(std::array<const char*, 5>{"Top", "Bottom", "Center", "Top", "Bottom"}, (uint8_t)old_yDriver, 5, "#position200", 30);
				if (old_yDriver != new_yDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetYDriver(new_yDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetYPosValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value3Speed = camSize * (component.GetPrimaryYFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}
				float value = component.GetYPosValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value5", &value, _value3Speed))
					component.SetYPosValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::PushFont(boldFont);
				ImGui::Text("Height");
				ImGui::PopFont();
				UITransform::HeightDriver heightDriver = component.GetHeightDriver();
				UITransform::HeightDriver newHeightDriver = (UITransform::HeightDriver)(SelectWidget(std::array<const char*, 3>{"Constant", "Relative", "Aspect"}, (uint8_t)heightDriver >> 4, 3, "#scale200") << 4);
				if (heightDriver != newHeightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetHeightDriver(newHeightDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetHeightValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value4Speed = camSize * (component.GetSecondaryYFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
				}
				value = component.GetHeightValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value6", &value, _value4Speed))
					component.SetHeightValue(value);
				ImGui::PopItemWidth();
			}
			else {
				ImGui::PushFont(boldFont);
				ImGui::Text("Top");
				ImGui::PopFont();
				UITransform::TopDriver old_topDriver = component.GetTopDriver();
				UITransform::TopDriver new_topDriver = (UITransform::TopDriver)SelectWidget(std::array<const char*, 2>{"Constant", "Relative"}, (uint8_t)old_topDriver, 2, "#top200");
				if (old_topDriver != new_topDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetTopDriver(new_topDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetTopValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value3Speed = camSize * (component.GetPrimaryYFromWorldPos(worldPos.x + 1) - newValue) / SLIDER_FRICTION;
				}

				float value = component.GetTopValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value7", &value, _value3Speed))
					component.SetTopValue(value);
				ImGui::PopItemWidth();
				ImGui::Spacing();

				ImGui::PushFont(boldFont);
				ImGui::Text("Bottom");
				ImGui::PopFont();
				UITransform::BottomDriver bottomDriver = component.GetBottomDriver();
				UITransform::BottomDriver newBottomDriver = (UITransform::BottomDriver)(SelectWidget(std::array<const char*, 2>{"Constant", "Relative"}, (uint8_t)bottomDriver >> 4, 2, "#bottom200") << 4);
				if (bottomDriver != newBottomDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetBottomDriver(newBottomDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetBottomValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
					_value4Speed = camSize * (component.GetSecondaryYFromWorldScale(worldScale.x + 1) - newValue) / SLIDER_FRICTION;
				}
				value = component.GetBottomValue();
				ImGui::PushItemWidth(ImGui::CalcItemWidth() * 1.5f);
				if (ImGui::DragFloat("##Value8", &value, _value4Speed))
					component.SetBottomValue(value);
				ImGui::PopItemWidth();
			}

			/*
			ImGui::Columns(1);

			ImGui::PushFont(boldFont);
			ImGui::Text("Horizontal");
			ImGui::PopFont();
			bool isTransformActive = !component.GetUseSidesHorizontal();
			bool transformActive = SelectWidget("Transform", "Sides", isTransformActive, true, "#transformActive");
			if (transformActive != isTransformActive) {
				const glm::vec2& worldPos = component.GetWorldPosition();
				const glm::vec2& worldScale = component.GetWorldScale();

				component.SetUseSidesHorizontal(!transformActive);
				float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();

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
					SelectWidget(std::array<const char*, 5>{"ConstLeft", "ConstRight", "RelativeCenter", "RelativeLeft", "RelativeRight"}, (uint8_t)xDriver, 5, "#position100");
				if (xDriver != newXDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetXDriver(newXDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetXPosValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
					(SelectWidget(std::array<const char*, 3>{"ConstWidth", "RelativeWidth", "AspectWidth"}, (uint8_t)widthDriver >> 4, 3, "#scale100") << 4);
				if (widthDriver != newSelectedWidth) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetWidthDriver(newSelectedWidth);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetWidthValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
					SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)leftDriver, 2, "#leftSide100");
				if (leftDriver != newLeftDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetLeftSideDriver(newLeftDriver);
					float newValue = component.GetPrimaryXFromWorldPos(worldPos.x);
					component.SetLeftSideValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
					(SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)rightDriver >> 4, 2, "#rightside") << 4);
				if (rightDriver != newRightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetRightSideDriver(newRightDriver);
					float newValue = component.GetSecondaryXFromWorldScale(worldScale.x);
					component.SetRightSideValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
			transformActive = SelectWidget("Transform", "Sides", isTransformActive, true, "#transformActive2");
			if (isTransformActive != transformActive) {
				const glm::vec2& worldPos = component.GetWorldPosition();
				const glm::vec2& worldScale = component.GetWorldScale();

				component.SetUseSidesVertical(!transformActive);
				float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();

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
				UITransform::YDriver newYDriver = (UITransform::YDriver)SelectWidget(std::array<const char*, 5>{"ConstTop", "ConstBottom", "Center", "RelativeTop", "RelativeBottom"}, (uint8_t)yDriver, 5, "#position200");
				if (yDriver != newYDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetYDriver(newYDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetYPosValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
				UITransform::HeightDriver newHeightDriver = (UITransform::HeightDriver)(SelectWidget(std::array<const char*, 3>{"ConstHeight", "RelativeHeight", "AspectHeight"}, (uint8_t)heightDriver >> 4, 3, "#scale200") << 4);
				if (heightDriver != newHeightDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetHeightDriver(newHeightDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetHeightValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
				UITransform::TopDriver newTopDriver = (UITransform::TopDriver)SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)topDriver, 2, "#top200");
				if (topDriver != newTopDriver) {
					const glm::vec2& worldPos = component.GetWorldPosition();
					component.SetTopDriver(newTopDriver);
					float newValue = component.GetPrimaryYFromWorldPos(worldPos.y);
					component.SetTopValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
				UITransform::BottomDriver newBottomDriver = (UITransform::BottomDriver)(SelectWidget(std::array<const char*, 2>{"ConstOffset", "RelativeOffset"}, (uint8_t)bottomDriver >> 4, 2, "#bottom200") << 4);
				if (bottomDriver != newBottomDriver) {
					const glm::vec2& worldScale = component.GetWorldScale();
					component.SetBottomDriver(newBottomDriver);
					float newValue = component.GetSecondaryYFromWorldScale(worldScale.y);
					component.SetBottomValue(newValue);
					float camSize = scene->GetPrimaryCamera().GetComponent<CameraComponent>().camera.GetSize();
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
			*/

			ImGui::Spacing();

			// World position node
			if (ImGui::TreeNodeEx("#worldPositionNode", ImGuiTreeNodeFlags_SpanAvailWidth, "World Position")) {
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

		DrawComponent<NativeScriptComponent>("Script component", drawedEntity, [&](NativeScriptComponent& comp) {
			if (comp.OnPropertyRenderFunc)
				comp.OnPropertyRenderFunc(comp.baseInstance);
			else
				ImGui::TextWrapped("Use OnPropertyRender() to render a property panel.");
		});

		if (drawedEntity.HasComponent<NativeScriptComponent>()) {
			NativeScriptComponent& script = drawedEntity.GetComponent<NativeScriptComponent>();

		}


		DrawComponent<CameraComponent>("Camera", drawedEntity, [&](CameraComponent& component) {
			if (ImGui::Button("Set primary camera")) {
				auto scene = Assets::GetScene(Application::Get().GetGameLayer()->GetActiveScene());
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
			comp.data.alignHorizontal = (TextAlignHorizontal)SelectWidget("Horizontal", std::array<const char*, 3>{"Left", "Middle", "Right"}, (uint8_t)comp.data.alignHorizontal, 4, "#textComponent", -30);
			comp.data.alignVertical = (TextAlignVertical)SelectWidget("Vertical", std::array<const char*, 3>{"Top", "Middle", "Bottom"}, (uint8_t)comp.data.alignVertical, 4, "#textComponent2", -30);

			ImGui::DragFloat("Font size", &comp.data.fontSize, 0.02f, 0, 100);

			ImGui::ColorEdit4("Color", glm::value_ptr(comp.data.color));
			ImGui::DragInt("Chars visible", (int*)&comp.data.charsVisible, 0.2f, -1, std::numeric_limits<int>::max());
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

			if (ImGui::TreeNodeEx("#Emitters2081", flags, "Emitters")) {
				auto& emitters = component.particleSystem.GetEmitters();
				for (auto& emitter : emitters) {
					float emitPerSec = emitter->GetEmistPerSecond();
					ImGui::Text("Emitting amount per second");
					if (ImGui::DragFloat("", &emitPerSec, emitPerSec / 500))
						emitter->SetEmitsPerSecond(emitPerSec);
					ImGui::Spacing();
					auto& generators = emitter->GetSetters();

					if (ImGui::TreeNodeEx("#generators329", flagsNotOpen, "Generators")) {
						for(auto& generator : generators)
							generator->OnImGuiRender();
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("#updaters10398", flags, "Updaters")) {
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