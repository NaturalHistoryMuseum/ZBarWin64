/*------------------------------------------------------------------------
 *  Copyright 2009 (c) Jeff Brown <spadix@users.sourceforge.net>
 *
 *  This file is part of the ZBar Bar Code Reader.
 *
 *  The ZBar Bar Code Reader is free software; you can redistribute it
 *  and/or modify it under the terms of the GNU Lesser Public License as
 *  published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  The ZBar Bar Code Reader is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser Public License
 *  along with the ZBar Bar Code Reader; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 *  Boston, MA  02110-1301  USA
 *
 *  http://sourceforge.net/projects/zbar
 *------------------------------------------------------------------------*/

#include "zbarmodule.h"

static PyObject*
exc_get_message (zbarException *self,
                 void *closure)
{
    PyBaseExceptionObject *super = (PyBaseExceptionObject*)self;
    if(!PyString_Size(super->message)) {
        Py_CLEAR(super->message);
        if(!self->obj || !zbarProcessor_Check(self->obj))
            super->message = PyString_FromString("unknown zbar error");
        else {
            const void *zobj = ((zbarProcessor*)self->obj)->zproc;
            super->message = PyString_FromString(_zbar_error_string(zobj, 1));
        }
    }
    Py_INCREF(super->message);
    return(super->message);
}

static int
exc_init (zbarException *self,
          PyObject *args,
          PyObject *kwds)
{
    PyBaseExceptionObject *super;
    if(!_PyArg_NoKeywords(self->base.ob_type->tp_name, kwds))
        return(-1);
    super = (PyBaseExceptionObject*)self;
    Py_CLEAR(super->args);
    Py_INCREF(args);
    super->args = args;

    if(PyTuple_GET_SIZE(args) == 1) {
        Py_CLEAR(self->obj);
        self->obj = PyTuple_GET_ITEM(args, 0);
        Py_INCREF(self->obj);
    }
    return(0);
}

static int
exc_traverse (zbarException *self,
              visitproc visit,
              void *arg)
{
    PyTypeObject *base;
    Py_VISIT(self->obj);
    base = (PyTypeObject*)PyExc_Exception;
    return(base->tp_traverse((PyObject*)self, visit, arg));
}

static int
exc_clear (zbarException *self)
{
    Py_CLEAR(self->obj);
    ((PyTypeObject*)PyExc_Exception)->tp_clear((PyObject*)self);
    return(0);
}

static void
exc_dealloc (zbarException *self)
{
    exc_clear(self);
    ((PyTypeObject*)PyExc_Exception)->tp_dealloc((PyObject*)self);
}

static PyObject*
exc_str (zbarException *self)
{
    return(exc_get_message(self, NULL));
}

static int
exc_set_message (zbarException *self,
                 PyObject *value,
                 void *closure)
{
    PyBaseExceptionObject *super = (PyBaseExceptionObject*)self;
    Py_CLEAR(super->message);
    if(!value)
        value = PyString_FromString("");
    else
        Py_INCREF(value);
    super->message = value;
    return(0);
}

static PyGetSetDef exc_getset[] = {
    { "message", (getter)exc_get_message, (setter)exc_set_message, },
    { NULL, },
};

PyTypeObject zbarException_Type = {
    PyObject_HEAD_INIT(NULL)
	0,                              /* ob_size */
    "zbar.Exception",               /* tp_name */
    sizeof(zbarException),          /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)exc_dealloc,        /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_compare */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    (reprfunc)exc_str,              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, /* tp_flags */
    0,                              /* tp_doc */
    (traverseproc)exc_traverse,     /* tp_traverse */
    (inquiry)exc_clear,             /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    0,                              /* tp_iter */
    0,                              /* tp_iternext */
    0,                              /* tp_methods */
    0,                              /* tp_members */
    exc_getset,                     /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)exc_init,             /* tp_init */
    0,                              /* tp_alloc */
    0,                              /* tp_new */
    0,                              /* tp_free */
    0,                              /* tp_is_gc*/
    0,                              /* tp_bases */
    0,                              /* tp_mro */
    0,                              /* tp_cache */
    0,                              /* tp_subclasses */
    0,                              /* tp_weaklist */
    0,                              /* tp_del */
    0                               /* tp_version_tag */
};

PyObject*
zbarErr_Set (PyObject *self)
{
    const void *zobj = ((zbarProcessor*)self)->zproc;
    zbar_error_t err = _zbar_get_error_code(zobj);

    if(err == ZBAR_ERR_NOMEM)
        PyErr_NoMemory();
    else if(err < ZBAR_ERR_NUM) {
        PyObject *type = zbar_exc[err];
        assert(type);
        PyErr_SetObject(type, self);
    }
    else
        PyErr_SetObject(zbar_exc[0], self);
    return(NULL);
}
