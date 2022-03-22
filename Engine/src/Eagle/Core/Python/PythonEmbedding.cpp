#include <EaglePCH.h>
#include "PythonEmbedding.h"

#ifdef _DEBUG
	#undef _DEBUG
	#include <Python.h>
	#define _DEBUG
#else
	#include <Python.h>
#endif

#include <filesystem>

#define SUPPORTED_PYTHON_ARGUMENT_CPP(TYPE, RETURN_FUNCTION) void* PythonEmbedding::GetArgumentPointer(TYPE& arg) const { return (RETURN_FUNCTION(arg)); }
#define SUPPORTED_PYTHON_ARGUMENT_CPP_CSTR(TYPE, RETURN_FUNCTION) void* PythonEmbedding::GetArgumentPointer(TYPE& arg) const { return (RETURN_FUNCTION(arg.c_str())); }

namespace Egl {

    /////////////////////////////
    //////// Python file ////////
    /////////////////////////////

    PythonFile::PythonFile(std::string filepath) : path(filepath) {
        FILE* filepoint;
        errno_t err;

        if ((err = fopen_s(&filepoint, filepath.c_str(), "r")) != 0) {
            exists = false;
            LOG_ENG_ERROR("Python: Cannot open file", filepath);
            return;
        }

        exists = true;
        file = filepoint;
    }
    PythonFile::~PythonFile() {
        if (!exists)
            return;

        if (pythonObj != nullptr)
            Py_DECREF(pythonObj);

        fclose(file);
    }

    ////////////////////////////////////
    //////// Python return data ////////
    ////////////////////////////////////

    PythonReturnData::PythonReturnData(void* data) {
        this->data = data;
    }
    PythonReturnData::~PythonReturnData() {
        if (data != NULL)
            Py_DECREF(data);
    }
    int PythonReturnData::GetDataAsInt() const {
        EAGLE_ENG_ASSERT(data != NULL, "Python: Trying to get null return data as int");
        return PyLong_AsLong((PyObject*)data);
    }
    float PythonReturnData::GetDataAsFloat() const {
        EAGLE_ENG_ASSERT(data != NULL, "Python: Trying to get null return data as float");
        return PyFloat_AsDouble((PyObject*)data);
    }
    std::string PythonReturnData::GetDataAsString() const {
        EAGLE_ENG_ASSERT(data != NULL, "Python: Trying to get null return data as string");
        return std::string(PyUnicode_AsUTF8((PyObject*)data));
    }

    //////////////////////////////////
    //////// Python embedding ////////
    //////////////////////////////////

    // Supported python arguments. Add it also to the .h file
    SUPPORTED_PYTHON_ARGUMENT_CPP_CSTR(const std::string, PyUnicode_FromString);
    SUPPORTED_PYTHON_ARGUMENT_CPP_CSTR(std::string, PyUnicode_FromString);
    SUPPORTED_PYTHON_ARGUMENT_CPP(const char*, PyUnicode_FromString);
    SUPPORTED_PYTHON_ARGUMENT_CPP(int, PyLong_FromLong);
    SUPPORTED_PYTHON_ARGUMENT_CPP(float, PyFloat_FromDouble);
    SUPPORTED_PYTHON_ARGUMENT_CPP(double, PyFloat_FromDouble);

    PythonEmbedding::PythonEmbedding() {
        if (Py_IsInitialized())
            return;
        initialized = true;

        Py_SetPythonHome(L"../Engine/vendor/Python310");
        Py_Initialize();
        pythonPaths = Py_GetPath();
        pythonPaths += L";../Build/Script.py"; // (std::wstring)std::filesystem::current_path() + std::wstring(L";") + pythonPaths;
        Py_SetPath(pythonPaths.c_str());
        std::wcout << Py_GetPath() << " "<< std::filesystem::current_path() << std::endl;
    }

    PythonEmbedding::~PythonEmbedding() {
        if (!initialized)
            return;

        Py_Finalize();
    }


    void* PythonEmbedding::SetupFunctionArguments(int& argCounter) const {
        // argCounter is the amount of args. They are inserted after this base function call.
        if (argCounter == 0)
            return nullptr;
        return PyTuple_New(argCounter);
    }

    void PythonEmbedding::InsertPythonObjectToTuple(void* tuple, void* object, int position) const {
        EAGLE_ENG_ASSERT(position < PyTuple_Size((PyObject*)tuple), "Python args: position index was bigger than tuple size.");
        PyTuple_SetItem((PyObject*)tuple, position, (PyObject*)object);
    }

    PythonReturnData PythonEmbedding::FinalizeCall(const std::string& filepath, const std::string& functionName, void* argumentTuple) const {
        std::string _filename = filepath.substr(filepath.length() - 3, 3) == std::string(".py") ? filepath.substr(0, filepath.length() - 3) : filepath;
        const std::string& _FunctionName = functionName;

        // Load file
        PyObject* pName = PyUnicode_FromString(_filename.c_str());
        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);
        if (pModule == nullptr) {
            DisplayError();
            return PythonReturnData(NULL);
        }

        // Load function
        PyObject* pFunc = PyObject_GetAttrString(pModule, _FunctionName.c_str());
        if (!pFunc || !PyCallable_Check(pFunc)) {
            DisplayError();
            return PythonReturnData(NULL);
        }

        PyObject* pArgs = (PyObject*)argumentTuple;
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs); // Run the function
        Py_DECREF(pArgs);
        Py_DECREF(pModule);

        return PythonReturnData(pValue);
    }

    void PythonEmbedding::RunFile(const std::string& filepath) const {
        FILE* PScriptFile;
        errno_t err;

        if ((err = fopen_s(&PScriptFile, filepath.c_str(), "r")) != 0) {
            LOG_ENG_ERROR("Python: Cannot open file", filepath);
            return;
        }

        PyRun_SimpleFile(PScriptFile, filepath.c_str());
        fclose(PScriptFile);
    }
    void PythonEmbedding::RunFile(const PythonFile& file) const {

    }

    void PythonEmbedding::DisplayError() const {
        if (!PyErr_Occurred()) {
            LOG_ERROR("Python: Error occured");
            return;
        }

        PyObject* ptype, * pvalue, * ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        
        const char* error = PyUnicode_AsUTF8(PyObject_Repr(pvalue));
        LOG_ERROR("Python:", error);
    }
}