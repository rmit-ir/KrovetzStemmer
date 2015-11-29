#include <Python.h>

#include <iostream>

#include "KrovetzStemmer.hpp"

typedef struct {
    PyObject_HEAD

    stem::KrovetzStemmer* stemmer;
    char buf[stem::KrovetzStemmer::MAX_WORD_LENGTH];
} KrovetzStemmer;

static PyObject*
KrovetzStemmer_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    KrovetzStemmer* self;
    
    self = (KrovetzStemmer*)type->tp_alloc(type, 0);
    if (self != NULL) {
	self->stemmer = new stem::KrovetzStemmer;
	if (self->stemmer == NULL) return NULL;
    }

    return (PyObject*)self;
}

static void
KrovetzStemmer_dealloc(KrovetzStemmer* self)
{
    delete self->stemmer;
    self->stemmer = NULL;

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject*
KrovetzStemmer_stem(KrovetzStemmer* self, PyObject* args) {
    PyObject* term;

    if (!PyArg_ParseTuple(args, "O", &term)) return NULL;

    if (!PyString_Check(term)) {
	PyErr_BadArgument();
	return NULL;
    }

    PyObject* stem = PyString_FromString(PyString_AsString(term));
    int processed = self->stemmer->kstem_stem_tobuffer(
	PyString_AS_STRING(stem), self->buf);

    if (processed > 0) {
	Py_DECREF(stem);
	return PyString_FromStringAndSize(self->buf, processed - 1);
    }
    else
	return stem;
}

static PyMethodDef KrovetzStemmer_methods[] = {
    {"stem", (PyCFunction)KrovetzStemmer_stem, METH_VARARGS,
	"Return the stem of the given term"},

    {NULL}    /* Sentinel */
};

static PyTypeObject KrovetzStemmerType = {
    PyObject_HEAD_INIT(NULL)

    0,                                  /* ob_size */
    "krovetzstemmer.KrovetzStemmer",    /* tp_name */
    sizeof(KrovetzStemmer),             /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)KrovetzStemmer_dealloc, /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    "KrovetzStemmer objects",           /* tp_doc */
     0,		                        /* tp_traverse */
    0,		                        /* tp_clear */
    0,		                        /* tp_richcompare */
    0,		                        /* tp_weaklistoffset */
    0,		                        /* tp_iter */
    0,		                        /* tp_iternext */
    KrovetzStemmer_methods,             /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    KrovetzStemmer_new,                 /* tp_new */
};

static PyMethodDef krovetzstemmer_methods[] = {
    {NULL}    /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initkrovetzstemmer(void) 
{
    if (PyType_Ready(&KrovetzStemmerType) < 0) return;

    PyObject* m = Py_InitModule3("krovetzstemmer", krovetzstemmer_methods, 
				 "Python binding to the KrovetzStemmer package 4.3 (C++ version)");
    if (m == NULL) return;

    Py_INCREF(&KrovetzStemmerType);
    PyModule_AddObject(m, "KrovetzStemmer", (PyObject*)&KrovetzStemmerType);
}
