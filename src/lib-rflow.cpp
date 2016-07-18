/******************************************************************************
* Copyright (C) 2016  Billy Kozak                                             *
*                                                                             *
* This file is part of lib-rflow                                              *
*                                                                             *
* This program is free software: you can redistribute it and/or modify        *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation, either version 3 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
******************************************************************************/
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include "lib-rflow.h"

#include "private/rflow.h"

#include <new>
#include <string>
#include <sstream>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
struct matrix_mem {
	struct rf_matrix matrix;
	unsigned bins[];
};
/******************************************************************************
*                            FUNCTION DEFINITIONS                             *
******************************************************************************/
static size_t cstr_copy(const std::string &s, char **copy)
{
	size_t len = s.size() + 1;
	*copy = (char*)malloc(len);

	if(*copy == NULL) {
		return 0;
	}

	memcpy(*copy, s.c_str(), len);
	return len;
}
/*****************************************************************************/
static size_t string_matrix(const struct rf_matrix *m, char **cstr_out)
{
	std::ostringstream stream;

	for(int i = 0; i < m->amp_bin_count; i++) {
		for(int n = 0; n < m->mean_bin_count; n++) {
			unsigned bin = m->bins[n + i * m->mean_bin_count];
			if(n != (m->mean_bin_count - 1)) {
				stream << bin << ", ";
			} else {
				stream << bin << std::endl;
			}
		}
	}

	return cstr_copy(stream.str(), cstr_out);
}
/*****************************************************************************/
extern "C"
struct rf_state* lib_rflow_init(const struct rf_init *init)
{
	size_t cells = init->amp_bin_count * init->mean_bin_count;

	struct matrix_mem *m = (struct matrix_mem*)malloc(
		sizeof(struct rf_matrix) + cells * sizeof(m->bins[0])
	);
	if(m == NULL) {
		goto fail;
	}

	m->matrix.amp_bin_count  = init->amp_bin_count;
	m->matrix.mean_bin_count = init->mean_bin_count;
	m->matrix.mean_min       = init->mean_min;
	m->matrix.amp_min        = init->amp_min;
	m->matrix.mean_bin_size  = init->mean_bin_size;
	m->matrix.amp_bin_size   = init->amp_bin_size;

	m->matrix.bins            = m->bins;

	try {
		return new rf_state(&m->matrix);
	} catch(std::bad_alloc& exc) {
		goto fail;
	} catch(...) {
		goto fail;
	}

fail:
	free(m);
	return NULL;
}
/*****************************************************************************/
extern "C"
int lib_rflow_count(struct rf_state *state, const double *points, size_t num)
{
	try {
		state->count(points, num);
		return 0;
	} catch(...) {
		return 1;
	}
}
/*****************************************************************************/
extern "C"
struct rf_matrix* lib_rflow_get_matrix(struct rf_state *state)
{
	try {
		return state->get_matrix();
	} catch(...) {
		return NULL;
	}
}
/*****************************************************************************/
extern "C"
void lib_rflow_destroy(struct rf_state *state)
{
	delete state;
}
/*****************************************************************************/
extern "C"
size_t lib_rflow_string_matrix(const struct rf_matrix *m, char **cstr_out)
{
	*cstr_out = NULL;
	try {
		return string_matrix(m, cstr_out);
	} catch(...) {
		free(cstr_out);
		return 0;
	};
}
/*****************************************************************************/