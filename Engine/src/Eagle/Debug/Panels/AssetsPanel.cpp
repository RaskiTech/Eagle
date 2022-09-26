#include <EaglePCH.h>
#include <inttypes.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/GLM.h>
#include "AssetsPanel.h"
//#include "Eagle/ECS/Components.h"
//#include "Eagle/ECS/ComponentsInternal.h"

namespace Egl {
	
	std::unordered_map<uint32_t, std::string> AssetsPanel::assetNames;

	void Egl::AssetsPanel::OnImGuiRender() {

		std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>> assetData = Assets::GetAllAssetData();

		ImGui::Begin("Assets viewer");

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushFont(boldFont);
		ImGui::Text("Asset Viewer");
		ImGui::PopFont();

		ImGui::Text("Loaded assets: %d", assetData.size());

		ImGui::Spacing();

		const int columnAmount = 4;
		if (ImGui::BeginTable("asset_table", columnAmount, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody)) {

			ImGui::TableSetupColumn("ID");
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Reference count");
			ImGui::TableSetupColumn("Location");
			ImGui::TableHeadersRow();

			for (int i = 0; i < assetData.size(); i++) {
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%" PRIu32 "\n", assetData[i].first); // ID
				ImGui::TableNextColumn();
				ImGui::Text(assetNames.count(assetData[i].first) == 0 ? "Unknown" : assetNames[assetData[i].first].c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%" PRIu32 "\n", assetData[i].second.first);
				ImGui::TableNextColumn();
				ImGui::Text("%p", assetData[i].second.second);
			}

			ImGui::EndTable();
		}

		ImGui::End();
	}

}
