#pragma once
#include <unordered_map>

namespace Egl {
	class AssetPanel {
	public:
		AssetPanel() = default;
		
		static void SubmitAssetName(uint32_t id, const std::string& name) { refreshData = true; assetNames[id] = name; }
		static void DeleteAssetName(uint32_t id) { refreshData = true; assetNames.erase(id); }
		static const std::string& GetAssetName(uint32_t id) { return assetNames[id]; }

		void OnImGuiRender();

	private:
		void RefreshData();

	private:
		// Choose some random number that aren't hopefully used by any other ImGui stuff
		enum class SortOption {
			Name = 232,
			ID = 233,
			ReferenceCount = 234
		};

		// Store these names only in edit mode
		static std::unordered_map<uint32_t, std::string> assetNames;
		
		static std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>> assetDataSorted;
		static bool refreshData;
		static SortOption sortWay;
		static bool sortDescending;
	};
}
