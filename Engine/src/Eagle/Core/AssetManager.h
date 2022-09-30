#pragma once
#include <memory>
#include <unordered_map>
#include <string>

// If space is needed could unload recources that aren't being used often even 
// if they have references, and then just load them again when they are used

namespace Egl {
	struct AudioClip;
	class Texture;
	class SubTexture;

	struct AssetReference {
		uint32_t id = -1;
		
		operator uint32_t() const { return id; }
		AssetReference() : id(-1) {}
		AssetReference(uint32_t id) : id(id) {}
		~AssetReference();
		AssetReference(const AssetReference& other);
		AssetReference& operator=(AssetReference other);
	};

	// These are all the types Assets will handle
	// Could use typedef, but since those are not actually new types they can cause function overload ambiguity
	class AudioClipRef : public AssetReference { public: AudioClipRef(uint32_t id) : AssetReference(id) {} AudioClipRef() : AssetReference() {} };
	class TextureRef    : public AssetReference { public: TextureRef(uint32_t id)    : AssetReference(id) {} TextureRef() : AssetReference() {} };
	class SubTextureRef : public AssetReference { public: SubTextureRef(uint32_t id) : AssetReference(id) {} SubTextureRef() : AssetReference() {} };

	class Assets {
	public:
		static AudioClipRef CreateClip(const std::string& filepath);
		static TextureRef CreateTexture(const std::string& filepath, bool scaleUpBlur = true, bool tile = false);
		static TextureRef CreateTexture(uint32_t width, uint32_t height, bool scaleUpBlur = true, bool tile = false, const char* shortIdentifierName = "");
		static SubTextureRef CreateSubTexture(const TextureRef& texture, const glm::vec2& oneCellSize, const glm::vec2& thisCellIndex, const glm::vec2& thisCellIndexSize);
		static SubTextureRef CreateSubTexture(const TextureRef& texture, const glm::vec2& minCoord, const glm::vec2& maxCoord);
		static TextureRef CreateFontAtlas(const std::string& filepath) { EAGLE_ENG_ASSERT(false, "Not implemented."); return -1; }

		static Texture* GetTexture(const TextureRef& id) { return (Texture*)assets[id].second; }
		static SubTexture* GetSubTexture(const SubTextureRef& id) { return (SubTexture*)assets[id].second; }
		static AudioClip* GetClip(const AudioClipRef& id) { return (AudioClip*)assets[id].second; }

		static std::vector<uint32_t> GetAllUsedIDs();
		static std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>> GetAllAssetData();
		
	private:
		friend AssetReference;

		// The map takes in an id, which could be any reference. The former in the pair is reference count, the latter is a pointer to the asset
		// Names are (currently) not stored, unless the editor is running. Then the names are stored in AssetsPanel
		static std::unordered_map<uint32_t, std::pair<uint32_t, void*>> assets;
		static uint32_t nextID;

		static uint32_t GetNextID();
		static void UnloadAsset(const AssetReference& ref);
	};
}