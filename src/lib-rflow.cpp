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
#include <stdlib.h>
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
	}

fail:
	free(m);
	return NULL;
}
/*****************************************************************************/
extern "C"
int lib_rflow_count(struct rf_state *state, const double *points, size_t num)
{
	state->count(points, num);
	return 0;
}
/*****************************************************************************/
extern "C"
struct rf_matrix* lib_rflow_get_matrix(struct rf_state *state)
{
	return state->get_matrix();
}
/*****************************************************************************/
extern "C"
void lib_rflow_destroy(struct rf_state *state)
{
	delete state;
}
/*****************************************************************************/