#include <EaglePCH.h>
#include "AssetManager.h"
#include "Audio.h"

namespace Egl {
    std::unordered_map<AudioClipID, AudioClip*> Assets::audioClips;
    std::unordered_map<TextureID, Texture*> Assets::textures;
    uint32_t Assets::nextID = 0;

    AudioClipID Assets::CreateClip(const std::string& filepath) {
        AudioClip* clip = new AudioClip(filepath);
        AudioClipID id = nextID;
        nextID++;
        audioClips[id] = clip;
        return id;
    }

    TextureID Assets::CreateTexture(const std::string& filepath) {
        LOG_ENG_ERROR("CreateTexture is not finished, use CreateRef<Texture>() in the meantime");
        return 0;
    }
    void Assets::UnloadClip(AudioClipID id) {
        LOG_ENG_ERROR("No unloading yet");
    }
    void Assets::UnloadTexture(TextureID id) {
        LOG_ENG_ERROR("No unloading yet");
    }
}