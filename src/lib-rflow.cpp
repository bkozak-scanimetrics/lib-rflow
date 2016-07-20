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
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
struct matrix_mem {
	struct rf_matrix matrix;
	unsigned bins[];
};
/*****************************************************************************/
struct lib_rflow_state {
public:
	uint32_t                         mode;
	std::unique_ptr<cycle_processor> proc;
	std::unique_ptr<rf_state>        rstate;
	struct rf_matrix                *matrix;

	lib_rflow_state(const struct lib_rflow_init *init);
};
/******************************************************************************
*                              STATIC FUNCTIONS                               *
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
static rf_matrix* build_matrix(const struct lib_rflow_init *init)
{
	uint32_t mode = init->opts & LIB_RFLOW_MODE_MASK;

	if(mode != LIB_RFLOW_MODE_MATRIX) {
		return NULL;
	}

	struct matrix_mem *m  = NULL;
	size_t cells =   init->mode_data.matrix_data.amp_bin_count
	               * init->mode_data.matrix_data.mean_bin_count;

	m = (struct matrix_mem*)malloc(
		sizeof(struct rf_matrix) + cells * sizeof(m->bins[0])
	);
	if(m == NULL) {
		throw std::bad_alloc{};
	}

	m->matrix.amp_bin_count  = init->mode_data.matrix_data.amp_bin_count;
	m->matrix.mean_bin_count = init->mode_data.matrix_data.mean_bin_count;
	m->matrix.mean_min       = init->mode_data.matrix_data.mean_min;
	m->matrix.amp_min        = init->mode_data.matrix_data.amp_min;
	m->matrix.mean_bin_size  = init->mode_data.matrix_data.mean_bin_size;
	m->matrix.amp_bin_size   = init->mode_data.matrix_data.amp_bin_size;

	m->matrix.bins            = m->bins;
	return &m->matrix;
}
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
lib_rflow_state::lib_rflow_state(const struct lib_rflow_init *init)
{
	mode = init->opts & LIB_RFLOW_MODE_MASK;

	custom_cycle_proc::processor  p_func;
	custom_cycle_proc::finisher   fini;
	void                        *state;

	matrix = build_matrix(init);

	try {
		switch(mode) {
		case LIB_RFLOW_MODE_PASSTHROUGH:
			proc.reset(new cycle_passthrough());
			break;
		case LIB_RFLOW_MODE_MATRIX:
			proc.reset(new matrix_counter(build_matrix(init)));
			break;
		case LIB_RFLOW_MODE_CUSTOM:
			p_func  = init->mode_data.custom_data.proc;
			fini    = init->mode_data.custom_data.fini;
			state   = init->mode_data.custom_data.state;
			proc.reset(new custom_cycle_proc(p_func, fini, state));
			break;
		default:
			throw std::runtime_error{"bad mode argument"};
			break;
		}

		rstate.reset(new rf_state(&*proc));

	} catch(const std::runtime_error &exc) {
		free(matrix);
		throw exc;
	} catch(...) {
		free(matrix);
		throw std::bad_alloc{};
	}
}
/******************************************************************************
*                              GLOBAL FUNCTIONS                               *
******************************************************************************/
extern "C"
struct lib_rflow_state* lib_rflow_init(const struct lib_rflow_init *init)
{
	try {
		return new lib_rflow_state(init);
	}  catch(std::bad_alloc& exc) {
		goto fail;
	} catch(...) {
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
		s->rstate->count(arr, num);
		return 0;
	} catch(...) {
		return 1;
	}
}
/*****************************************************************************/
extern "C"
struct rf_matrix* lib_rflow_get_matrix(struct lib_rflow_state *s)
{
	try {
		return s->matrix;
	} catch(...) {
		return NULL;
	}
}
/*****************************************************************************/
extern "C"
size_t lib_rflow_pop_cycles(struct lib_rflow_state *s,
                            struct lib_rflow_cycle **p)
{
	if(s->mode != LIB_RFLOW_MODE_PASSTHROUGH) {
		return 0;
	}
	try {
		auto *pt = static_cast<cycle_passthrough*>(&*s->proc);
		return pt->pop_cycle_list(p);
	}  catch(...) {
		return 0;
	}
}
/*****************************************************************************/
extern "C"
size_t lib_rflow_pop_cycles_replace_mem(
	struct lib_rflow_state *s, struct lib_rflow_cycle **p,
	struct lib_rflow_cycle *new_mem,  size_t new_mem_size
)
{
	if(s->mode != LIB_RFLOW_MODE_PASSTHROUGH) {
		return 0;
	}
	try {
		auto *pt = static_cast<cycle_passthrough*>(&*s->proc);
		return pt->pop_cycle_list(p);
	}  catch(...) {
		return 0;
	}
}
/*****************************************************************************/
extern "C"
int lib_rflow_end_history(struct lib_rflow_state *s)
{
	try {
		s->rstate->terminate();
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