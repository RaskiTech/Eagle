#pragma once
#include <EaglePCH.h>
#include <EagleBuildSettings.h>
#include <filesystem>
#include "Core.h"

static std::string SetPersistantPath();

namespace Egl {
	class Serialize {
	public:
		/// <summary>Save a class that can then be loaded into a file.</summary>
		template<typename T>
		static void Save(const std::string& saveFilePath, const T& classData) {
			LOG_WARN("Saving is currantly under construction...");
			std::string path = persistantDataPath + saveFilePath;
			
		}

	protected:
		static std::string persistantDataPath;
		static void Init() {
			persistantDataPath = SetPersistantPath();
		}
		friend class Application;
	};
}

#ifdef EAGLE_PLATFORM_WINDOWS
	#include <ShlObj.h>
	static std::string SetPersistantPath() {
		PWSTR pathTmp;
		auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pathTmp);
		if (get_folder_path_ret != S_OK) {
			CoTaskMemFree(pathTmp);
			LOG_ENG_ERROR("Couldn't get persistant data path. Saving isn't available.");
			return "";
		}
		std::filesystem::path path(pathTmp);
		CoTaskMemFree(pathTmp);

		std::stringstream ss;
		ss << path.string() << '\\' << APPLICATION_NAME << '\\';
		return ss.str();
	}
#endif