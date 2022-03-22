#pragma once

//*
// Supported python arguments. Add it also to the cpp file
#define SUPPORTED_PYTHON_ARGUMENT_H(TYPE) \
template<> void* GetPythonObjectPointer<TYPE>(TYPE& type) const { return GetArgumentPointer(type); } \
void* GetArgumentPointer(TYPE& arg) const;
//*/

namespace Egl {

	struct PythonFile {
		PythonFile(std::string filepath);
		~PythonFile();

	private:
		friend class PythonEmbedding;
		std::string path;
		FILE* file = nullptr;
		void* pythonObj = nullptr;
		bool exists = false;
	};
	struct PythonReturnData {
		PythonReturnData(void* data);
		~PythonReturnData();

		bool HasValue() const { return data != NULL; }

		const void* GetDataPointer() const { return data; }
		int GetDataAsInt() const;
		float GetDataAsFloat() const;
		std::string GetDataAsString() const;

	private:
		void* data;
	};

	class PythonEmbedding {
	public:
		PythonEmbedding();
		~PythonEmbedding();

		PythonFile GetFile(const std::string& filepath) const { return PythonFile(filepath); }
		void RunFile(const std::string& filepath) const;
		void RunFile(const PythonFile& file) const;

		template<typename... Args>
		PythonReturnData RunFunction(const std::string& filepath, const std::string& functionName, Args&&... args) const {
			int argCounter = 0; // Used to count the args in the recursive loop
			void* packedArgs = SetupFunctionArguments(argCounter, args...);
			return FinalizeCall(filepath, functionName, packedArgs);
		}

		void DisplayError() const;

	private:
		//*
		template<typename TFirst, typename... TRest>
		void* SetupFunctionArguments(int& argCounter, TFirst& first, TRest&&... rest) const {
			int thisArgumentIndex = argCounter;
			argCounter++; // Count the args
			void* argsTuple = SetupFunctionArguments(argCounter, rest...);
			InsertPythonObjectToTuple(argsTuple, GetPythonObjectPointer(first), thisArgumentIndex);
			return argsTuple;
		}
		void* SetupFunctionArguments(int& argCounter) const;
		void InsertPythonObjectToTuple(void* tuple, void* object, int position) const;
		PythonReturnData FinalizeCall(const std::string& filepath, const std::string& functionName, void* argumentTuple) const;

		template<typename T> void* GetPythonObjectPointer(T& type) const { LOG_ERROR("Python: Unsupported argument type."); return GetPythonObjectPointer(std::string("Unsupported argument")); }

		// Supported python arguments. Add it also to the .cpp file
		SUPPORTED_PYTHON_ARGUMENT_H(const std::string);
		SUPPORTED_PYTHON_ARGUMENT_H(std::string);
		SUPPORTED_PYTHON_ARGUMENT_H(const char*);
		SUPPORTED_PYTHON_ARGUMENT_H(int);
		SUPPORTED_PYTHON_ARGUMENT_H(float);
		SUPPORTED_PYTHON_ARGUMENT_H(double);
		//*/

	private:

		std::wstring pythonPaths;
		bool initialized = false;
	};
}