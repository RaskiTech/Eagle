#include <EaglePCH.h>
#include <inttypes.h>
#include <Dependencies/ImGui.h>
#include <Dependencies/GLM.h>
#include "AssetsPanel.h"
#include "Eagle/Debug/EditorLayer.h"

namespace Egl {
	
	std::unordered_map<uint32_t, std::string> AssetPanel::assetNames;
	std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>> AssetPanel::assetDataSorted = std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>>();
	bool AssetPanel::refreshData = true;
	bool AssetPanel::sortDescending = true;
	AssetPanel::SortOption AssetPanel::sortWay;

	void Egl::AssetPanel::OnImGuiRender() {

		ImGui::Begin("Assets viewer");

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushFont(boldFont);
		ImGui::Text("Asset Viewer");
		ImGui::PopFont();

		ImGui::Text("Loaded assets: %d", assetDataSorted.size());

		ImGui::Spacing();

		const int columnAmount = 4;
		if (ImGui::BeginTable("asset_table", columnAmount, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Sortable)) {

			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, -1, (int)AssetPanel::SortOption::Name);
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 0.5f, (int)AssetPanel::SortOption::ID);
			ImGui::TableSetupColumn("Reference count", ImGuiTableColumnFlags_WidthFixed, -1, (int)AssetPanel::SortOption::ReferenceCount);
			ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_WidthFixed, -1);
			//ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableHeadersRow();

			ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();
			if (sortSpecs != nullptr) {
				if (sortSpecs->SpecsDirty) {
					
					if (AssetPanel::sortWay != (AssetPanel::SortOption)sortSpecs->Specs->ColumnUserID || (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) != sortDescending) {
						AssetPanel::sortWay = (AssetPanel::SortOption)sortSpecs->Specs->ColumnUserID; // The id defined in TableSetupColumn
						sortDescending = sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending;

						refreshData = true;
						sortSpecs->SpecsDirty = false;
					}
				}
			}

			if (refreshData) {
				RefreshData();
			}

			auto& assetDataSorted = AssetPanel::assetDataSorted;

			for (int i = 0; i < assetDataSorted.size(); i++) {
				// It is possible one asset in the array is id -1, because that is the default id if nothing is assigned. This can do whatever it likes
				if (assetDataSorted[i].first == -1)
					continue;

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text(assetNames.count(assetDataSorted[i].first) == 0 ? "Unknown" : assetNames[assetDataSorted[i].first].c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%" PRIu32 "\n", assetDataSorted[i].first); // ID
				ImGui::TableNextColumn();
				ImGui::Text("%" PRIu32 "\n", assetDataSorted[i].second.first);
				ImGui::TableNextColumn();
				ImGui::Text("%p", assetDataSorted[i].second.second);
			}

			ImGui::EndTable();
		}

		ImGui::End();
	}

	bool CompareEntryID       (bool ascending, const std::pair<uint32_t, std::pair<uint32_t, void*>> o1, const std::pair<uint32_t, std::pair<uint32_t, void*>> o2) { if (o1.first == o2.first) return false; return ascending == (o1.first > o2.first); }
	bool CompareEntryName     (bool ascending, const std::pair<uint32_t, std::pair<uint32_t, void*>> o1, const std::pair<uint32_t, std::pair<uint32_t, void*>> o2) { if (AssetPanel::GetAssetName(o1.first) == AssetPanel::GetAssetName(o2.first)) return false; return ascending == (AssetPanel::GetAssetName(o1.first) > AssetPanel::GetAssetName(o2.first)); }
	bool CompareEntryReference(bool ascending, const std::pair<uint32_t, std::pair<uint32_t, void*>> o1, const std::pair<uint32_t, std::pair<uint32_t, void*>> o2) { if (o1.second.first == o2.second.first) return false; return ascending == (o1.second.first > o2.second.first); }
	void AssetPanel::RefreshData() {
		assetDataSorted = Assets::GetAllAssetData();

		auto sort = CompareEntryID;
		if (sortWay == SortOption::Name)
			sort = CompareEntryName;
		else if (sortWay == SortOption::ReferenceCount)
			sort = CompareEntryReference;

		auto sortAscendingOrDescending = [&](const std::pair<uint32_t, std::pair<uint32_t, void*>> o1, const std::pair<uint32_t, std::pair<uint32_t, void*>> o2) { return sort(sortDescending, o1, o2); };

		std::sort(assetDataSorted.begin(), assetDataSorted.end(), sortAscendingOrDescending);

		refreshData = false;
	}

}
