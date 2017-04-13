#include <Python.h>
#include <bytesobject.h>

#include <iostream>

#include "KrovetzStemmer.hpp"

typedef struct {
    PyObject_HEAD

    stem::KrovetzStemmer* stemmer;
    char buf[stem::KrovetzStemmer::MAX_WORD_LENGTH];
} Stemmer;

static PyObject*
Stemmer_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
    Stemmer* self;
    
    self = (Stemmer*)type->tp_alloc(type, 0);
    if (self != NULL) {
	self->stemmer = new stem::KrovetzStemmer;
	if (self->stemmer == NULL) return NULL;
    }

    return (PyObject*)self;
}

static void
Stemmer_dealloc(Stemmer* self)
{
    delete self->stemmer;
    self->stemmer = NULL;

    /* self->ob_type->tp_free((PyObject*)self); */
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
Stemmer_stem(Stemmer* self, PyObject* args) {
    PyObject* term;
    if (!PyArg_ParseTuple(args, "O", &term)) return NULL;

    int is_unicode = PyUnicode_Check(term);

    PyObject* stem;
    if (is_unicode) 
	stem = PyUnicode_AsUTF8String(term); 
    else if (PyBytes_Check(term))
	stem = PyBytes_FromString(PyBytes_AS_STRING(term));
    else {
	PyErr_BadArgument();
	return NULL;
    }

    int processed = self->stemmer->kstem_stem_tobuffer(
	PyBytes_AS_STRING(stem), self->buf);

    if (processed > 0) {
	Py_DECREF(stem);

	if (is_unicode) 
	    return PyUnicode_DecodeUTF8(self->buf, processed - 1, NULL);
	else
	    return PyBytes_FromStringAndSize(self->buf, processed - 1);
    }
    else {
	if (is_unicode) {
	    PyObject* unicode = PyUnicode_FromString(PyBytes_AS_STRING(stem));
	    Py_DECREF(stem);
	    return unicode;
	}
	else 
	    return stem;
    }
}

static PyObject*
Stemmer_call(Stemmer* self, PyObject* args, PyObject* kwargs) {
    return Stemmer_stem(self, args);
}

static PyMethodDef Stemmer_methods[] = {
    {"stem", (PyCFunction)Stemmer_stem, METH_VARARGS, "Return the stem of the given term"},
    {NULL, NULL}    /* Sentinel */
};

static PyTypeObject StemmerType = {
    PyVarObject_HEAD_INIT(NULL, 0)

    "krovetzstemmer.Stemmer",    /* tp_name */
    sizeof(Stemmer),             /* tp_basicsize */
    0,                           /* tp_itemsize */
    (destructor)Stemmer_dealloc, /* tp_dealloc */
    0,                           /* tp_print */
    0,                           /* tp_getattr */
    0,                           /* tp_setattr */
    0,                           /* tp_compare */
    0,                           /* tp_repr */
    0,                           /* tp_as_number */
    0,                           /* tp_as_sequence */
    0,                           /* tp_as_mapping */
    0,                           /* tp_hash */
    (ternaryfunc)Stemmer_call,   /* tp_call */
    0,                           /* tp_str */
    0,                           /* tp_getattro */
    0,                           /* tp_setattro */
    0,                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,          /* tp_flags */
    "KrovetzStemmer objects",    /* tp_doc */
    0,		                 /* tp_traverse */
    0,		                 /* tp_clear */
    0,		                 /* tp_richcompare */
    0,		                 /* tp_weaklistoffset */
    0,		                 /* tp_iter */
    0,		                 /* tp_iternext */
    Stemmer_methods,             /* tp_methods */
    0,                           /* tp_members */
    0,                           /* tp_getset */
    0,                           /* tp_base */
    0,                           /* tp_dict */
    0,                           /* tp_descr_get */
    0,                           /* tp_descr_set */
    0,                           /* tp_dictoffset */
    0,                           /* tp_init */
    0,                           /* tp_alloc */
    Stemmer_new,                 /* tp_new */
};

static PyMethodDef krovetzstemmer_methods[] = {
    {NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,  /* m_base */
    "krovetzstemmer",       /* m_name */
    NULL,                   /* m_doc */
    -1,                     /* m_size */
    krovetzstemmer_methods  /* m_methods */
};
#endif

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
PyInit_krovetzstemmer(void)
{
    if (PyType_Ready(&StemmerType) < 0) return NULL;
    PyObject* m = PyModule_Create(&moduledef);
    if (m == NULL) return NULL;
#else
initkrovetzstemmer(void) 
{
    if (PyType_Ready(&StemmerType) < 0) return;
    PyObject* m = Py_InitModule3("krovetzstemmer", krovetzstemmer_methods, 
				 "Python binding to the KrovetzStemmer package 4.3 (C++ version)");
    if (m == NULL) return;
#endif

    Py_INCREF(&StemmerType);
    PyModule_AddObject(m, "Stemmer", (PyObject*)&StemmerType);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
