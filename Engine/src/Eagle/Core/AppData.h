#pragma once
#include <EaglePCH.h>
#include <filesystem>
#include "Core.h"

static std::string SetPersistantPath();

#define EGL_SAVEDATA_CHECK() EAGLE_ENG_ASSERT(valid, "SaveData: Attempted to add data to a file that wasn't valid. Check is it valid before writing.")
#define EGL_LOADDATA_CHECK() EAGLE_ENG_ASSERT(valid, "LoadData: Attempted to get data from a file that wasn't valid. Check is it valid before reading.")

namespace Egl {
	/// <summary>Save data for later use. Note: the data must be read in the same order.</summary>
	class SaveData {
	public:
		SaveData(const std::string& relativeLoadPath);
		~SaveData();
	
		inline bool IsValid() { return valid; }

		inline void AddData(int data) { EGL_SAVEDATA_CHECK(); file.write((const char*)&data, sizeof(int)); }
		inline void AddData(uint32_t data) { EGL_SAVEDATA_CHECK(); file.write((const char*)&data, sizeof(uint32_t)); }
		inline void AddData(float data) { EGL_SAVEDATA_CHECK(); file.write((const char*)&data, sizeof(float)); }
		inline void AddData(double data) { EGL_SAVEDATA_CHECK(); file.write((const char*)&data, sizeof(double)); }
		inline void AddData(bool data) { EGL_SAVEDATA_CHECK(); file.write((const char*)&data, sizeof(bool)); }
		inline void AddChar(char data) { EGL_SAVEDATA_CHECK(); file.write((const char*)&data, sizeof(char)); }
		inline void AddData(const char* data) { AddData((std::string)data); }
		inline void AddData(const std::string& data) { 
			EGL_SAVEDATA_CHECK(); 
			uint32_t length = (uint32_t)data.size();
			AddData(length);
			file.write(data.c_str(), length);
		}
		void Save();
	
	protected:
		static void Init() {
			persistantDataPath = SetPersistantPath();
		}
		static std::string persistantDataPath;
		friend class Application;
		friend class LoadData;
	private:
		bool valid;
		std::ofstream file;
	};

	class LoadData {
	public:
		LoadData(const std::string& relativeLoadPath);
		~LoadData();
	
		inline bool IsValid() { return valid; }

		inline int         GetInt() {      EGL_LOADDATA_CHECK();      int t; file.read((char*)&t, sizeof(int)); return t; }
		inline uint32_t    GetUint32_t() { EGL_LOADDATA_CHECK(); uint32_t t; file.read((char*)&t, sizeof(uint32_t)); return t; }
		inline float       GetFloat() {    EGL_LOADDATA_CHECK();    float t; file.read((char*)&t, sizeof(float)); return t; }
		inline double      GetDouble() {   EGL_LOADDATA_CHECK();   double t; file.read((char*)&t, sizeof(double)); return t; }
		inline bool        GetBool() {     EGL_LOADDATA_CHECK();     bool t; file.read((char*)&t, sizeof(bool)); return t; }
		inline char        GetChar() {     EGL_LOADDATA_CHECK();     char t; file.read((char*)&t, sizeof(char)); return t; }
		inline std::string GetString() { 
			EGL_LOADDATA_CHECK();
			uint32_t len = GetUint32_t();
			char* temp = new char[len + 1];
			file.read(temp, len);
			temp[len] = '\0';
			std::string str = temp;
			delete[] temp;
			return str;
		}

	private:
		bool valid;
		std::ifstream file;
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
		LOG("Save path:", ss.str());
		return ss.str();
	}
#endif