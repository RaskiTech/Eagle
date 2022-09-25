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
        std::string moduleName = PythonEmbedding::GetModuleName(filepath);

        PyObject* pName = PyUnicode_FromString(moduleName.c_str());
        pythonObj = PyImport_Import(pName);
        Py_DECREF(pName);
        if (pythonObj == nullptr) {
            PythonEmbedding::DisplayError();
            exists = false;
            return;
        }

        exists = true;
    }
    PythonFile::~PythonFile() {
        if (!exists)
            return;

        if (pythonObj != nullptr)
            Py_DECREF(pythonObj);
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
        return (float)PyFloat_AsDouble((PyObject*)data);
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

        // Add current dir and assets dir in python search paths
        // Note: While it is recommended that we call the Eagle python api functions with Assets/ prefix, it is not needed, since it gets stripped away anyway (apart from 1 function).
        // TODO: Figure out how to import modules (aka. files) with relative paths. Below we are hardcoding the Assets folder, meaning it's the only folder the scripts will work on.
        PyRun_SimpleString("import sys\nsys.path.append(\".\")\nsys.path.append(\"./Assets\")");
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
        const std::string moduleName = GetModuleName(filepath);

        // Load file
        PyObject* pName = PyUnicode_FromString(moduleName.c_str());
        PyObject* pModule = PyImport_Import(pName);
        Py_DECREF(pName);
        if (pModule == nullptr) {
            DisplayError();
            return PythonReturnData(NULL);
        }

        PythonReturnData data = FinalizeCall(pModule, functionName, argumentTuple);
        Py_DECREF(pModule);
        return data;
    }

    PythonReturnData PythonEmbedding::FinalizeCall(void* pythonFileObj, const std::string& functionName, void* argumentTuple) const {

        // Load function
        PyObject* pFunc = PyObject_GetAttrString((PyObject*)pythonFileObj, functionName.c_str());
        if (!pFunc || !PyCallable_Check(pFunc)) {
            DisplayError();
            return PythonReturnData(NULL);
        }

        PyObject* pArgs = (PyObject*)argumentTuple;
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs); // Run the function
        if (pArgs != nullptr)
			Py_DECREF(pArgs);

        if (pValue == nullptr) {
            LOG_ENG_ERROR("Python: The following is either a python runtime error or the function couldn't be called.");
            DisplayError();
            return PythonReturnData(NULL);
        }

        return PythonReturnData(pValue);
    }

    void PythonEmbedding::RunFile(const std::string& filepath) const {
        
        PyObject* obj = Py_BuildValue("s", filepath.c_str());

        // "Note: On Windows, fp should be opened as binary mode (e.g. fopen(filename, "rb")). Otherwise, Python may not handle script file with LF line ending correctly."
        FILE* file = _Py_fopen_obj(obj, "rb");

        if (file == nullptr) {
            DisplayError();
            //LOG_ENG_ERROR("Python: Error loading", filepath, "-", strerror(err));
            return;
        }

        int ret = PyRun_SimpleFileEx(file, filepath.c_str(), 1);
    }
    void PythonEmbedding::RunFile(const PythonFile& file) const {
        RunFile(file.GetPath());
    }

    void PythonEmbedding::DisplayError() {
        if (!PyErr_Occurred()) {
            LOG_ENG_ERROR("Python: Error occured");
            return;
        }

        PyObject* ptype, * pvalue, * ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        
        const char* error = PyUnicode_AsUTF8(PyObject_Repr(pvalue));
        LOG_ENG_ERROR("Python:", error);
    }
}