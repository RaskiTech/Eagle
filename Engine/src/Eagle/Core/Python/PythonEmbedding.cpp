#include <EaglePCH.h>
#include "PythonEmbedding.h"

#ifdef _DEBUG
	#undef _DEBUG
	#include <Python.h>
	#define _DEBUG
#else
	#include <Python.h>
#endif

#define SUPPORTED_PYTHON_ARGUMENT_CPP(TYPE, RETURN_FUNCTION) void* PythonEmbedding::GetArgumentPointer(TYPE& arg) const { return (RETURN_FUNCTION(arg)); }
#define SUPPORTED_PYTHON_ARGUMENT_CPP_CSTR(TYPE, RETURN_FUNCTION) void* PythonEmbedding::GetArgumentPointer(TYPE& arg) const { return (RETURN_FUNCTION(arg.c_str())); }

namespace Egl {
    /*
    void* PythonEmbedding::GetArgumentPointer(const std::string& arg) const { return PyUnicode_FromString(arg.c_str()); }
    void* PythonEmbedding::GetArgumentPointer(std::string& arg) const { return PyUnicode_FromString(arg.c_str()); }
    void* PythonEmbedding::GetArgumentPointer(const char*& arg) const { return PyUnicode_FromString(arg); }
    void* PythonEmbedding::GetArgumentPointer(int& arg) const { return PyLong_FromLong(arg); }
    void* PythonEmbedding::GetArgumentPointer(float& arg) const { return PyFloat_FromDouble(arg); }
    void* PythonEmbedding::GetArgumentPointer(double& arg) const { return PyFloat_FromDouble(arg); }
    /*/
    // Supported python arguments. Add it also to the .h file
    SUPPORTED_PYTHON_ARGUMENT_CPP_CSTR(const std::string, PyUnicode_FromString);
    SUPPORTED_PYTHON_ARGUMENT_CPP_CSTR(std::string, PyUnicode_FromString);
    SUPPORTED_PYTHON_ARGUMENT_CPP(const char*, PyUnicode_FromString);
    SUPPORTED_PYTHON_ARGUMENT_CPP(int, PyLong_FromLong);
    SUPPORTED_PYTHON_ARGUMENT_CPP(float, PyFloat_FromDouble);
    SUPPORTED_PYTHON_ARGUMENT_CPP(double, PyFloat_FromDouble);
    //*/

    PythonFile::PythonFile(std::string filepath) : path(filepath) {
        file = fopen(filepath.c_str(), "r");
        if (!file) {
            exists = false;
            return;
        }

        exists = true;

    }
    PythonFile::~PythonFile() {
        if (!exists)
            return;
        fclose(file);
    }

    PythonEmbedding::PythonEmbedding() {
        if (Py_IsInitialized())
            return;
        initialized = true;

        Py_SetPythonHome(L"../Engine/vendor/Python310");
        Py_Initialize();
        pythonPaths = Py_GetPath();
        pythonPaths += L";../Build";
        Py_SetPath(pythonPaths.c_str());
        //std::wcout << Py_GetPath() << " "<< std::filesystem::current_path() << std::endl;
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
            LOG_ERROR("Python: Couldn't find file");
            return PythonReturnData(NULL);
        }

        // Load function
        PyObject* pFunc = PyObject_GetAttrString(pModule, "InputFunction");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            LOG_ERROR("Python: Error getting function");
            if (PyErr_Occurred())
                PyErr_Print();
            return PythonReturnData(NULL);
        }

        PyObject* pArgs = (PyObject*)argumentTuple;
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs); // Run the function
        Py_DECREF(pArgs);
        Py_DECREF(pModule);

        return PythonReturnData(pValue);
    }

    void PythonEmbedding::RunFile(const std::string& filepath) const {
        FILE* PScriptFile = fopen(filepath.c_str(), "r");
        if (PScriptFile) {
            PyRun_SimpleFile(PScriptFile, filepath.c_str());
            fclose(PScriptFile);
        }
        else {
            LOG_WARN("Couldn't open file: ", filepath);
        }
    }
    void PythonEmbedding::RunFile(const PythonFile& file) const {

    }

    PythonReturnData::PythonReturnData(void* data) {
        this->data = data;
    }
    PythonReturnData::~PythonReturnData() {
        if (data != NULL)
            Py_DECREF(data);
    }
    int PythonReturnData::GetDataAsInt() {
        EAGLE_ENG_ASSERT(data != NULL, "Getting return data even though it's null");
        return PyLong_AsLong((PyObject*)data);
    }
}