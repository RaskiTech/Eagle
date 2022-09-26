#pragma once
#include <unordered_map>

namespace Egl {
	class AssetsPanel {
	public:
		AssetsPanel() = default;
		
		static void SubmitAssetName(uint32_t id, const std::string& name) { assetNames[id] = name; }
		static void DeleteAssetName(uint32_t id) { assetNames.erase(id); }
		static const std::string& GetAssetName(uint32_t id) { return assetNames[id]; }

		void OnImGuiRender();

	private:
		static std::unordered_map<uint32_t, std::string> assetNames;
	};
}
