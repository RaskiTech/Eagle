#pragma once
#include "PythonEmbedding.h"
#include "../Application.h"

// Static wrapper for PythonEmbedding

namespace Egl {
	class Python {
	public:
		static PythonFile GetFile(const std::string& filepath) { return Application::Get().GetPython().GetFile(filepath); }

		static void RunFile(const std::string& filepath) { Application::Get().GetPython().RunFile(filepath); }
		static void RunFile(const PythonFile& file) { Application::Get().GetPython().RunFile(file); }

		template<typename... Args>
		static void RunFunction(const std::string& filepath, const std::string& functionName, Args&&... args) {
			Application::Get().GetPython().RunFunction(filepath, functionName, args...);
		}
	};
}