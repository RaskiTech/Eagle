#include <EaglePCH.h>
#include "Eagle/Core/Core.h"
#include "AssetManager.h"
#include "Eagle/Debug/Panels/AssetsPanel.h"
#include "Audio.h"

namespace Egl {
    std::unordered_map<uint32_t, std::pair<uint32_t, void*>> Assets::assets;
    uint32_t Assets::nextID = 0;

    AssetReference::AssetReference(const AssetReference& other) : id(other.id) {
        if (id == -1)
            return;
		Assets::assets[id].first++;
    }
    AssetReference::~AssetReference() {
        if (id == -1)
            return;
		Assets::assets[id].first--;

        if (Assets::assets[id].first == 0)
            Assets::UnloadAsset(*this);
    }
	AssetReference& AssetReference::operator=(AssetReference other) {
		std::swap(id, other.id);
        //if (id != -1)
		//	Assets::assets[id].first++;
		return *this;
	}
    void Assets::UnloadAsset(const AssetReference& ref) {
        EAGLE_EDITOR_ONLY(AssetPanel::DeleteAssetName(ref));
        delete assets[ref].second;
        assets.erase(ref);
    }


    AudioClipRef Assets::CreateClip(const std::string& filepath) {
        AudioClip* asset = new AudioClip(filepath);
        AudioClipRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, std::string("Clip ").append(filepath.substr(filepath.find_last_of("/\\") + 1))));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    TextureRef Assets::CreateTexture(const std::string& filepath, bool scaleUpBlur, bool tile) {
        Texture* asset = Texture::CreateDangling(filepath, scaleUpBlur, tile);
        TextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, std::string("Tex ").append(filepath.substr(filepath.find_last_of("/\\") + 1))));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }
    TextureRef Assets::CreateTexture(uint32_t width, uint32_t height, bool scaleUpBlur, bool tile, const char* shortIdentifierName) {
        Texture* asset = Texture::CreateDangling(width, height, scaleUpBlur, tile);
        TextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, shortIdentifierName + std::string("Tex ").append(std::to_string(width)).append("x").append(std::to_string(height))));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    SubTextureRef Assets::CreateSubTexture(const TextureRef& texture, const glm::vec2& minCoord, const glm::vec2& maxCoord) {
        SubTexture* asset = SubTexture::CreateDangling(texture, minCoord, maxCoord);
        SubTextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Sub" + AssetPanel::GetAssetName(texture)));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }
    SubTextureRef Assets::CreateSubTexture(const TextureRef& texture, const glm::vec2& oneCellSize, const glm::vec2& thisCellIndex, const glm::vec2& thisCellIndexSize) {
        SubTexture* asset = SubTexture::CreateDanglingFromIndexes(texture, thisCellIndex, oneCellSize, thisCellIndexSize);
        SubTextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Sub" + AssetPanel::GetAssetName(texture)));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

	uint32_t Assets::GetNextID() {
        // Find and return the next free key
        while (assets.count(nextID) != 0) nextID++;
        return nextID++;
    }

    std::vector<uint32_t> Assets::GetAllUsedIDs() {
        std::vector<uint32_t> container;
        container.reserve(assets.size());
        for (auto& keyValue : assets)
            container.push_back(keyValue.first);
        return container;
    }
    std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>> Assets::GetAllAssetData() {
        std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>> container;
        container.reserve(assets.size());
        for (auto& keyValue : assets)
            container.push_back(keyValue);
        return container;
    }
}