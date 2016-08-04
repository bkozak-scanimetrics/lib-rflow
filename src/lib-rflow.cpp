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
#include "private/cycle-processor.h"
#include "private/cycle-passthrough.h"
#include "private/matrix-counter.h"
#include "private/custom-cycle-proc.h"

#include <new>
#include <string>
#include <sstream>
#include <memory>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
struct matrix_mem {
	struct lib_rflow_matrix matrix;
	unsigned bins[];
};
/*****************************************************************************/
struct lib_rflow_state {
public:
	const uint32_t                   mode;
	std::unique_ptr<cycle_processor> proc;
	rf_state                         rstate;

	lib_rflow_state(const struct lib_rflow_init *init,
	                struct lib_rflow_matrix *m);
};
/******************************************************************************
*                              STATIC FUNCTIONS                               *
******************************************************************************/
static uint32_t extract_mode(const struct lib_rflow_init *init)
{
	return init->opts & LIB_RFLOW_MODE_MASK;
}
/*****************************************************************************/
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
static size_t string_matrix(const struct lib_rflow_matrix *m, char **cstr_out)
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
static int build_matrix(struct lib_rflow_init *init,
                        struct lib_rflow_matrix **matrix)
{
	struct matrix_mem *m  = NULL;
	size_t cells =   init->mode_data.matrix_data.amp_bin_count
	               * init->mode_data.matrix_data.mean_bin_count;

	m = (struct matrix_mem*)calloc(
		sizeof(struct lib_rflow_matrix) + cells * sizeof(m->bins[0]),
		1
	);
	if(m == NULL) {
		return 1;
	}

	m->matrix.amp_bin_count  = init->mode_data.matrix_data.amp_bin_count;
	m->matrix.mean_bin_count = init->mode_data.matrix_data.mean_bin_count;
	m->matrix.mean_min       = init->mode_data.matrix_data.mean_min;
	m->matrix.amp_min        = init->mode_data.matrix_data.amp_min;
	m->matrix.mean_bin_size  = init->mode_data.matrix_data.mean_bin_size;
	m->matrix.amp_bin_size   = init->mode_data.matrix_data.amp_bin_size;

	m->matrix.bins            = m->bins;

	*matrix = &m->matrix;

	return 0;
}
/*****************************************************************************/
cycle_processor *build_processor(const struct lib_rflow_init *init,
                                 struct lib_rflow_matrix *matrix)
{
	uint32_t mode = extract_mode(init);
	custom_cycle_proc::processor proc;
	custom_cycle_proc::finisher  fini;
	void     *state;

	switch(mode) {
	case LIB_RFLOW_MODE_PASSTHROUGH:
		return new cycle_passthrough();
	case LIB_RFLOW_MODE_MATRIX:
		return new matrix_counter(matrix);
	case LIB_RFLOW_MODE_CUSTOM:
		proc    = init->mode_data.custom_data.proc;
		fini    = init->mode_data.custom_data.fini;
		state   = init->mode_data.custom_data.state;
		return new custom_cycle_proc(proc, fini, state);
	default:
		throw std::runtime_error{"bad mode argument"};
		break;
	}
	return NULL;
}
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
lib_rflow_state::lib_rflow_state(const struct lib_rflow_init *init,
                                  struct lib_rflow_matrix *m)
: mode{extract_mode(init)}, proc{build_processor(init, m)}, rstate{&*proc}
{
}
/******************************************************************************
*                              GLOBAL FUNCTIONS                               *
******************************************************************************/
extern "C"
struct lib_rflow_state* lib_rflow_init(struct lib_rflow_init *init)
{
	uint32_t mode = extract_mode(init);
	struct lib_rflow_matrix *matrix = NULL;

	if(mode == LIB_RFLOW_MODE_MATRIX) {
		if(build_matrix(init, &matrix)) {
			errno = ENOMEM;
			goto fail;
		}
	}

	try {
		return new lib_rflow_state(init, matrix);
	}  catch(std::bad_alloc& exc) {
		errno = ENOMEM;
		goto fail;
	} catch(...) {
		errno = EINVAL;
		goto fail;
	}

fail:
	return NULL;
}
/*****************************************************************************/
extern "C"
int lib_rflow_count(struct lib_rflow_state *s, const double *arr, size_t num)
{
	try {
		s->rstate.count(arr, num);
		return 0;
	} catch(...) {
		return 1;
	}
}
/*****************************************************************************/
extern "C"
const struct lib_rflow_matrix* lib_rflow_get_matrix(struct lib_rflow_state *s)
{
	if(s->mode != LIB_RFLOW_MODE_MATRIX) {
		return NULL;
	}

	try {
		auto mc = static_cast<matrix_counter*>(&*s->proc);
		return mc->get_matrix();
	} catch(...) {
		return NULL;
	}
}
/*****************************************************************************/
extern "C"
int lib_rflow_pop_cycles(struct lib_rflow_state *s, struct lib_rflow_list *l)
{
	if(s->mode != LIB_RFLOW_MODE_PASSTHROUGH) {
		return 1;
	}
	try {
		auto *pt = static_cast<cycle_passthrough*>(&*s->proc);
		*l = pt->pop_cycle_list();
		return 0;
	}  catch(...) {
		return 1;
	}
}
/*****************************************************************************/
extern "C"
int lib_rflow_pop_cycles_replace_mem(
	struct lib_rflow_state *s, struct lib_rflow_list *l,
	const struct lib_rflow_list *new_mem
)
{
	if(s->mode != LIB_RFLOW_MODE_PASSTHROUGH) {
		return 1;
	}
	try {
		auto *pt = static_cast<cycle_passthrough*>(&*s->proc);
		*l = pt->pop_cycle_list(new_mem);
		return 0;
	}  catch(...) {
		return 1;
	}
}
/*****************************************************************************/
extern "C"
int lib_rflow_end_history(struct lib_rflow_state *s)
{
	try {
		s->rstate.terminate();
		return 0;
	} catch(...) {
		return 1;
	}
}
/*****************************************************************************/
extern "C"
void lib_rflow_destroy(struct lib_rflow_state *s)
{
	delete s;
}
/*****************************************************************************/
extern "C"
size_t lib_rflow_string_matrix(const struct lib_rflow_matrix *m,
                               char **cstr_out)
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