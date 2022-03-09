#include "EaglePCH.h"
#include "PythonEmbedding.h"

#ifdef _DEBUG
	#undef _DEBUG
	#include "Python.h"
	#define _DEBUG
#else
	#include "Python.h"
#endif

namespace Egl {
	void Python::PythonTest() {
		Py_SetPythonHome(L"../Engine/vendor/Python310");
		Py_Initialize();

        int vectorAmount = 0;

		PyRun_SimpleString("from time import time,ctime\nprint('Python is working. Today is', ctime(time()))\n");

		/* File
		FILE* fp = _Py_wfopen(L"../../../Application/src/script.py", L"r");
		
        PyObject* pName, * pModule, * pFunc;
        PyObject* pArgs, * pValue;

        pName = PyUnicode_DecodeFSDefault("../../../Application/src/script.py");
        // Error checking of pName left out

        pModule = PyImport_Import(pName);
        Py_DECREF(pName);

        if (pModule != NULL) {
            pFunc = PyObject_GetAttrString(pModule, argv[2]);
            // pFunc is a new reference

            if (pFunc && PyCallable_Check(pFunc)) {
                // Make the input to the function
                pArgs = PyTuple_New(vectorAmount);
                for (int i = 0; i < vectorAmount; i++) {
                    pValue = PyLong_FromLong(atoi(argv[i + 3]));
                    if (!pValue) {
                        Py_DECREF(pArgs);
                        Py_DECREF(pModule);
                        fprintf(stderr, "Cannot convert argument\n");
                        return;
                    }
                    // pValue reference stolen here:
                    PyTuple_SetItem(pArgs, i, pValue);
                }
                pValue = PyObject_CallObject(pFunc, pArgs);
                Py_DECREF(pArgs);
                if (pValue != NULL) {
                    printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                    Py_DECREF(pValue);
                }
                else {
                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    PyErr_Print();
                    fprintf(stderr, "Call failed\n");
                    return;
                }
            }
            else {
                if (PyErr_Occurred())
                    PyErr_Print();
                fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
            }
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
            return 1;
        }

		//*/

		Py_Finalize();
	}
	void Python::RunFile() {

	}
	void Python::RunFunction() {

	}
}