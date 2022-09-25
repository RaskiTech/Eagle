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
		static PythonReturnData RunFunction(const std::string& filepath, const std::string& functionName, Args&&... args) {
			return Application::Get().GetPython().RunFunction(filepath, functionName, args...);
		}

		template<typename... Args>
		static PythonReturnData RunFunction(const PythonFile& file, const std::string& functionName, Args&&... args) {
			return Application::Get().GetPython().RunFunction(file, functionName, args...);
		}
	};
}