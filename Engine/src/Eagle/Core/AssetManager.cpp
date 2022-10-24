#include <EaglePCH.h>
#include "Eagle/Core/Core.h"
#include "AssetManager.h"
#include "Eagle/Debug/Panels/AssetsPanel.h"
#include "Eagle/ECS/Scene.h"
#include "Eagle/Rendering/Text/TextRenderer.h"
#include "Audio.h"

namespace Egl {
    std::unordered_map<uint32_t, std::pair<uint32_t, void*>> Assets::assets;
    uint32_t Assets::nextID = 0;

    void AssetReference::CopyConstructor(const AssetReference& other) {
        if (other.id == -1)
            return;
        id = other.id;
		Assets::assets[id].first++;
    }
    template<typename AssetType>
    void Assets::UnloadAsset(const AssetReference& ref) {
        EAGLE_EDITOR_ONLY(AssetPanel::DeleteAssetName(ref));
        delete (AssetType*)assets[ref].second;
        assets.erase(ref);
    }


    AudioClipRef Assets::LoadClip(const std::string& filepath) {
        EAGLE_PROFILE_FUNCTION();
        AudioClip* asset = new AudioClip(filepath);
        if (asset->data.GetAudioFileFormat() == AudioFileFormat::NotLoaded) {
            delete asset;
            return AudioClipRef();
        }

        AudioClipRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, std::string("Clip ").append(filepath.substr(filepath.find_last_of("/\\") + 1))));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    TextureRef Assets::LoadTexture(const std::string& filepath, bool scaleUpBlur, bool tile) {
        EAGLE_PROFILE_FUNCTION();
        Texture* asset = Texture::CreateDangling(filepath, scaleUpBlur, tile);
        TextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, std::string("Tex ").append(filepath.substr(filepath.find_last_of("/\\") + 1))));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }
    TextureRef Assets::LoadTexture(uint32_t width, uint32_t height, bool scaleUpBlur, bool tile, const char* shortIdentifierName) {
        EAGLE_PROFILE_FUNCTION();
        Texture* asset = Texture::CreateDangling(width, height, scaleUpBlur, tile);
        TextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, shortIdentifierName + std::string("Tex ").append(std::to_string(width)).append("x").append(std::to_string(height))));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    SubTextureRef Assets::LoadSubTexture(const TextureRef& texture, const glm::vec2& minCoord, const glm::vec2& maxCoord) {
        EAGLE_PROFILE_FUNCTION();
        SubTexture* asset = SubTexture::CreateDangling(texture, minCoord, maxCoord);
        SubTextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Sub" + AssetPanel::GetAssetName(texture)));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    ShaderRef Assets::LoadShader(const std::string& filepath) {
        EAGLE_PROFILE_FUNCTION();
        Shader* asset = Shader::CreateDangling(filepath);
        ShaderRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Shader " + asset->GetName()));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }
    ShaderRef Assets::LoadShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader) {
        EAGLE_PROFILE_FUNCTION();
        Shader* asset = Shader::CreateDangling(name, vertexShader, fragmentShader);
        ShaderRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Shader " + asset->GetName()));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    SubTextureRef Assets::LoadSubTexture(const TextureRef& texture, const glm::vec2& oneCellSize, const glm::vec2& thisCellIndex, const glm::vec2& thisCellIndexSize) {
        EAGLE_PROFILE_FUNCTION();
        SubTexture* asset = SubTexture::CreateDanglingFromIndexes(texture, thisCellIndex, oneCellSize, thisCellIndexSize);
        SubTextureRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Sub" + AssetPanel::GetAssetName(texture)));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    FontRef Assets::LoadFont(const std::string& name)
    {
        EAGLE_PROFILE_FUNCTION();
        FontRenderer* asset = new FontRenderer(name);
        FontRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Font " + name));
        assets[ref] = std::make_pair(1, asset);
        return ref;
    }

    SceneRef Assets::AddSceneToAssets(Scene* scenePtr) {
        SceneRef ref = GetNextID();
        EAGLE_EDITOR_ONLY(AssetPanel::SubmitAssetName(ref, "Scene"));
        assets[ref] = std::make_pair(1, scenePtr);
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