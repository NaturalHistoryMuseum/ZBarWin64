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

static char symboliter_doc[] = PyDoc_STR(
    "symbol iterator.\n"
    "\n"
    "iterates over decode results attached to an image.");

static int
symboliter_traverse (zbarSymbolIter *self,
                     visitproc visit,
                     void *arg)
{
    Py_VISIT(self->syms);
    return(0);
}

static int
symboliter_clear (zbarSymbolIter *self)
{
    if(self->zsym) {
        zbar_symbol_t *zsym = (zbar_symbol_t*)self->zsym;
        self->zsym = NULL;
        zbar_symbol_ref(zsym, -1);
    }
    Py_CLEAR(self->syms);
    return(0);
}

static void
symboliter_dealloc (zbarSymbolIter *self)
{
    symboliter_clear(self);
    ((PyObject*)self)->ob_type->tp_free((PyObject*)self);
}

static zbarSymbolIter*
symboliter_iter (zbarSymbolIter *self)
{
    Py_INCREF(self);
    return(self);
}

static zbarSymbol*
symboliter_iternext (zbarSymbolIter *self)
{
    zbar_symbol_t *zsym;
    if(self->zsym) {
        zbar_symbol_t *zsym = (zbar_symbol_t*)self->zsym;
        zbar_symbol_ref(zsym, -1);
        self->zsym = zbar_symbol_next(self->zsym);
    }
    else if(self->syms->zsyms)
        self->zsym = zbar_symbol_set_first_symbol(self->syms->zsyms);
    else
        self->zsym = NULL;

    zsym = (zbar_symbol_t*)self->zsym;
    if(!zsym)
        return(NULL);
    zbar_symbol_ref(zsym, 1);
    return(zbarSymbol_FromSymbol(self->zsym));
}

PyTypeObject zbarSymbolIter_Type = {
    PyObject_HEAD_INIT(NULL)
	0,                              /* ob_size */
    "zbar.SymbolIter",              /* tp_name */
    sizeof(zbarSymbolIter),         /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)symboliter_dealloc, /* tp_dealloc */
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
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, /* tp_flags */
    symboliter_doc,                 /* tp_doc */
    (traverseproc)symboliter_traverse, /* tp_traverse */
    (inquiry)symboliter_clear,      /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    (getiterfunc)symboliter_iter,   /* tp_iter */
    (iternextfunc)symboliter_iternext, /* tp_iternext */
    0,                              /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    0,                              /* tp_init */
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

zbarSymbolIter*
zbarSymbolIter_FromSymbolSet (zbarSymbolSet *syms)
{
    zbarSymbolIter *self;
    self = PyObject_GC_New(zbarSymbolIter, &zbarSymbolIter_Type);
    if(!self)
        return(NULL);

    Py_INCREF(syms);
    self->syms = syms;
    self->zsym = NULL;
    return(self);
}
