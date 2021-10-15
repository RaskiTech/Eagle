#include <EaglePCH.h>
#include <filesystem>
#include "Serialization.h"

namespace Egl {
	SaveData::SaveData(const std::string& relativeLoadPath) {
		std::string path = persistantDataPath + relativeLoadPath;
		if (!std::filesystem::exists(path))
			std::filesystem::create_directory(path.substr(0, path.find_last_of("\\/")));
		file.open(path, std::ios::binary);
		valid = (bool)file;
	}
	
	SaveData::~SaveData() {
		if (valid) file.close();
	}
	void SaveData::Save() {
		file.close();
		valid = false;
	}
	
	// LoadData //
	
	LoadData::LoadData(const std::string& relativeLoadPath) {
		std::string path = SaveData::persistantDataPath + relativeLoadPath;
		file.open(path, std::ios::binary);
		valid = (bool)file;
	}
	LoadData::~LoadData() {
		if (valid) file.close();
	}
}
