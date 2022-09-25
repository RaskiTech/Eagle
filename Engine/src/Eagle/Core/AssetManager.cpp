#include <EaglePCH.h>
#include "AssetManager.h"
#include "Audio.h"

namespace Egl {
    std::unordered_map<uint32_t, std::pair<uint32_t, void*>> Assets::assets;
    uint32_t Assets::nextID = 0;

    AssetReference::AssetReference(const AssetReference& other) : id(other.id) { Assets::assets[id].first++; }
    AssetReference::~AssetReference() { Assets::assets[id].first--; if (Assets::assets[id].first == 0) Assets::UnloadAsset(*this); }
	AssetReference& AssetReference::operator=(AssetReference other) {
		std::swap(id, other.id);
		Assets::assets[id].first++;
		return *this;
	}

    AudioClipRef Assets::CreateClip(const std::string& filepath) {
        AudioClip* asset = new AudioClip(filepath);
        AudioClipRef ref = GetNextID();
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    TextureRef Assets::CreateTexture(const std::string& filepath, bool scaleUpBlur, bool tile) {
        Texture* asset = Texture::CreateDangling(filepath, scaleUpBlur, tile);
        TextureRef ref = GetNextID();
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }
    TextureRef Assets::CreateTexture(uint32_t width, uint32_t height, bool scaleUpBlur, bool tile) {
        Texture* asset = Texture::CreateDangling(width, height, scaleUpBlur, tile);
        TextureRef ref = GetNextID();
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    SubTextureRef Assets::CreateSubTexture(TextureRef texture, const glm::vec2& minCoord, const glm::vec2& maxCoord) {
        SubTexture* asset = SubTexture::CreateDangling(texture, minCoord, maxCoord);
        SubTextureRef ref = GetNextID();
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }
    SubTextureRef Assets::CreateSubTexture(TextureRef texture, const glm::vec2& oneCellSize, const glm::vec2& thisCellIndex, const glm::vec2& thisCellIndexSize) {
        SubTexture* asset = SubTexture::CreateDanglingFromIndexes(texture, thisCellIndex, oneCellSize, thisCellIndexSize);
        SubTextureRef ref = GetNextID();
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    void Assets::UnloadAsset(const AssetReference& ref) {
        delete assets[ref].second;
        assets.erase(ref);
    }
}