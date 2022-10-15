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
	class Shader;
	class Scene;

	struct AssetReference {
		uint32_t id = -1;
		
		AssetReference() : id(-1) {}
		AssetReference(uint32_t id) : id(id) {}

		operator uint32_t() const { return id; }
		void CopyConstructor(const AssetReference& other);

		template<typename AssetType>
		void OnDelete() {
			if (id == -1)
				return;
			Assets::assets[id].first--;

			if (Assets::assets[id].first == 0)
				Assets::UnloadAsset<AssetType>(*this);
		}
	};

	#define DEFINE_ASSET_REFERENCE(ReferenceType, AssetType) \
		class ReferenceType : public AssetReference { \
			public: \
				ReferenceType() : AssetReference() {} \
				ReferenceType(uint32_t id) : AssetReference(id) {} \
				ReferenceType(const ReferenceType& other) { CopyConstructor(other); } \
				ReferenceType& operator=(ReferenceType other) { std::swap(id, other.id); return *this; } \
				~ReferenceType() { OnDelete<AssetType>(); } \
				operator uint32_t() const { return id; } \
		}





	// These are all the types Assets will handle
	// Could use typedef, but since those are not actually new types they can cause function overload ambiguity
	DEFINE_ASSET_REFERENCE(AudioClipRef, AudioClip);
	DEFINE_ASSET_REFERENCE(TextureRef, Texture);
	DEFINE_ASSET_REFERENCE(SubTextureRef, SubTexture);
	DEFINE_ASSET_REFERENCE(ShaderRef, Shader);
	DEFINE_ASSET_REFERENCE(SceneRef, Scene);

	class Assets {
	public:
		static AudioClipRef CreateClip(const std::string& filepath);
		static TextureRef CreateTexture(const std::string& filepath, bool scaleUpBlur = true, bool tile = false);
		static TextureRef CreateTexture(uint32_t width, uint32_t height, bool scaleUpBlur = true, bool tile = false, const char* shortIdentifierName = "");
		static SubTextureRef CreateSubTexture(const TextureRef& texture, const glm::vec2& oneCellSize, const glm::vec2& thisCellIndex, const glm::vec2& thisCellIndexSize);
		static SubTextureRef CreateSubTexture(const TextureRef& texture, const glm::vec2& minCoord, const glm::vec2& maxCoord);
		static ShaderRef CreateShader(const std::string& filepath);
		static ShaderRef CreateShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader);
		template<typename MyScene> static SceneRef CreateScene() { return AddSceneToAssets(new MyScene()); }
	private:
		static SceneRef AddSceneToAssets(Scene* sceneRef);
	public:
		//static TextureRef CreateFontAtlas(const std::string& filepath) { EAGLE_ENG_ASSERT(false, "Not implemented."); return -1; }

		static Texture* GetTexture(const TextureRef& id) { return (Texture*)assets[id].second; }
		static SubTexture* GetSubTexture(const SubTextureRef& id) { return (SubTexture*)assets[id].second; }
		static AudioClip* GetClip(const AudioClipRef& id) { return (AudioClip*)assets[id].second; }
		static Shader* GetShader(const ShaderRef& id) { return (Shader*)assets[id].second; }
		static Scene* GetScene(const SceneRef& id) { return (Scene*)assets[id].second; }

		static uint32_t GetReferenceCount(const AssetReference& ref) { return assets[ref].first; }
		static uint32_t GetReferenceCount_DEBUG(uint32_t id) { return assets[id].first; }
		static std::vector<uint32_t> GetAllUsedIDs();
		static std::vector<std::pair<uint32_t, std::pair<uint32_t, void*>>> GetAllAssetData();
		
	private:
		friend AssetReference;

		// The map takes in an id, which could be any reference. The former in the pair is reference count, the latter is a pointer to the asset
		// Names are (currently) not stored, unless the editor is running. Then the names are stored in AssetsPanel
		static std::unordered_map<uint32_t, std::pair<uint32_t, void*>> assets;
		static uint32_t nextID;

		static uint32_t GetNextID();
		template<typename AssetType> static void UnloadAsset(const AssetReference& ref);
	};
}