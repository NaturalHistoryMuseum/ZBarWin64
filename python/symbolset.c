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

static char symbolset_doc[] = PyDoc_STR(
    "symbol result container.\n"
    "\n"
    "collection of symbols.");

static int
symbolset_clear (zbarSymbolSet *self)
{
    if(self->zsyms) {
        zbar_symbol_set_t *zsyms = (zbar_symbol_set_t*)self->zsyms;
        self->zsyms = NULL;
        zbar_symbol_set_ref(zsyms, -1);
    }
    return(0);
}

static void
symbolset_dealloc (zbarSymbolSet *self)
{
    symbolset_clear(self);
    ((PyObject*)self)->ob_type->tp_free((PyObject*)self);
}

static zbarSymbolIter*
symbolset_iter (zbarSymbolSet *self)
{
    return(zbarSymbolIter_FromSymbolSet(self));
}

Py_ssize_t
symbolset_length (zbarSymbolSet *self)
{
    if(self->zsyms)
        return(zbar_symbol_set_get_size(self->zsyms));
    return(0);
}

static PySequenceMethods symbolset_as_sequence = {
    (lenfunc)symbolset_length,
    0,                              /* sq_concat */
    0,                              /* sq_repeat */
    0,                              /* sq_item */
    0,                              /* sq_ass_item */
    0,                              /* sq_contains */
    0,                              /* sq_inplace_concat */
    0,                              /* sq_inplace_repeat */
};

PyTypeObject zbarSymbolSet_Type = {
    PyObject_HEAD_INIT(NULL)
	0,                              /* ob_size */
    "zbar.SymbolSet",               /* tp_name */
    sizeof(zbarSymbolSet),          /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)symbolset_dealloc,  /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_compare */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    &symbolset_as_sequence,         /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    symbolset_doc,                  /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    (getiterfunc)symbolset_iter,    /* tp_iter */
    0,                              /* tp_iternext */
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

zbarSymbolSet*
zbarSymbolSet_FromSymbolSet(const zbar_symbol_set_t *zsyms)
{
    zbarSymbolSet *self = PyObject_New(zbarSymbolSet, &zbarSymbolSet_Type);
    if(!self)
        return(NULL);
    if(zsyms) {
        zbar_symbol_set_t *ncsyms = (zbar_symbol_set_t*)zsyms;
        zbar_symbol_set_ref(ncsyms, 1);
    }
    self->zsyms = zsyms;
    return(self);
}
