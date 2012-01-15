/*
 * This file is part of the sigrok project.
 *
 * Copyright (C) 2012 Bert Vermeulen <bert@biot.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sigrokdecode.h" /* First, so we avoid a _POSIX_C_SOURCE warning. */
#include "sigrokdecode-internal.h"
#include "config.h"


/* lives in type_decoder.c */
extern PyTypeObject srd_Decoder_type;

/* lives in type_logic.c */
extern PyTypeObject srd_logic_type;


/* When initialized, a reference to this module inside the python interpreter
 * lives here.
 */
PyObject *mod_sigrokdecode = NULL;

static struct PyModuleDef sigrokdecode_module = {
	PyModuleDef_HEAD_INIT,
	.m_name = "sigrokdecode",
	.m_doc = "sigrokdecode module",
	.m_size = -1,
};


PyMODINIT_FUNC PyInit_sigrokdecode(void)
{
	PyObject *mod;

	/* tp_new needs to be assigned here for compiler portability */
	srd_Decoder_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&srd_Decoder_type) < 0)
		return NULL;

	srd_logic_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&srd_logic_type) < 0)
		return NULL;

	mod = PyModule_Create(&sigrokdecode_module);
	Py_INCREF(&srd_Decoder_type);
	if (PyModule_AddObject(mod, "Decoder", (PyObject *)&srd_Decoder_type) == -1)
		return NULL;
	Py_INCREF(&srd_logic_type);
	if (PyModule_AddObject(mod, "srd_logic", (PyObject *)&srd_logic_type) == -1)
		return NULL;

	/* expose output types as symbols in the sigrokdecode module */
	if(PyModule_AddObject(mod, "OUTPUT_ANN",
			PyLong_FromLong(SRD_OUTPUT_ANN)) == -1)
		return NULL;
	if(PyModule_AddObject(mod, "OUTPUT_PROTO",
			PyLong_FromLong(SRD_OUTPUT_PROTO)) == -1)
		return NULL;
	if(PyModule_AddObject(mod, "OUTPUT_BINARY",
			PyLong_FromLong(SRD_OUTPUT_BINARY)) == -1)
		return NULL;

	mod_sigrokdecode = mod;

	return mod;
}

