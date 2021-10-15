#pragma once
#include <memory>
#include <unordered_map>
#include <string>

// If space is needed could unload recources that aren't being used often even 
// if they have references, and then just load them again when they are used

namespace Egl {
	struct AudioClip;
	class Texture;

	// Include reference counting?
	typedef uint32_t AudioClipID;
	typedef uint32_t TextureID;

	class Assets {
	public:
		static AudioClipID CreateClip(const std::string& filepath);
		static TextureID CreateTexture(const std::string& filepath);

		static Texture* GetTexture(TextureID id) { return textures[id]; }
		static AudioClip* GetClip(AudioClipID id) { return audioClips[id]; }

	private:
		static std::unordered_map<AudioClipID, AudioClip*> audioClips;
		static std::unordered_map<TextureID, Texture*> textures;
		static uint32_t nextID;

		static void UnloadClip(AudioClipID id);
		static void UnloadTexture(TextureID id);
	};
}