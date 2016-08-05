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
#ifndef LIB_RFLOW_H_
#define LIB_RFLOW_H_
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
/******************************************************************************
*                                   DEFINES                                   *
******************************************************************************/
#if __GNUC__
#define EXPORT __attribute__((visibility("default")))
#else
#error "compiler not supported"
#endif

#define LIB_RFLOW_MODE_MASK         0x03
#define LIB_RFLOW_MODE_PASSTHROUGH  0x00
#define LIB_RFLOW_MODE_MATRIX       0x01
#define LIB_RFLOW_MODE_CUSTOM       0x02

#define LIB_RFLOW_EMPTY_LIST {NULL, 0, 0}
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
struct lib_rflow_cycle {
	double cycle_start;
	double cycle_end;
};
/*****************************************************************************/
struct lib_rflow_list {
	struct lib_rflow_cycle *cycles;
	size_t                  num_cycles;
	size_t                  mem_size;
};
/*****************************************************************************/
struct lib_rflow_matrix {
	int      amp_bin_count;
	int      mean_bin_count;
	double   mean_min;
	double   amp_min;
	double   mean_bin_size;
	double   amp_bin_size;

	unsigned *bins;
};
/*****************************************************************************/
struct lib_rflow_init {

	uint32_t opts;

	union {
		struct {
			int amp_bin_count;
			int mean_bin_count;

			double mean_min;
			double amp_min;
			double mean_bin_size;
			double amp_bin_size;
		} matrix_data;
		struct {
			void (*proc)(const struct lib_rflow_cycle *, void*);
			void (*fini)(void*);
			void *state;
		} custom_data;
	} mode_data;
};
/*****************************************************************************/
struct lib_rflow_state;
/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/
EXPORT
struct lib_rflow_state* lib_rflow_init(struct lib_rflow_init *init);
EXPORT
int lib_rflow_count(struct lib_rflow_state *s, const double *arr, size_t num);
EXPORT
const struct lib_rflow_matrix* lib_rflow_get_matrix(struct lib_rflow_state *s);
EXPORT
void lib_rflow_destroy(struct lib_rflow_state *s);
EXPORT
size_t lib_rflow_string_matrix(const struct lib_rflow_matrix *m,
                               char **cstr_out);
EXPORT
int lib_rflow_pop_cycles(struct lib_rflow_state *s, struct lib_rflow_list *l);
EXPORT
int lib_rflow_pop_cycles_replace_mem(
	struct lib_rflow_state *s, struct lib_rflow_list *l,
	const struct lib_rflow_list *new_mem
);
EXPORT
int lib_rflow_end_history(struct lib_rflow_state *s);
EXPORT
size_t lib_rflow_cycle_list_size(const struct lib_rflow_state *s);
/******************************************************************************
*                              STATIC FUNCTIONS                               *
******************************************************************************/
static inline void lib_rflow_get_empty_list(struct lib_rflow_list *l)
{
	struct lib_rflow_list empty = LIB_RFLOW_EMPTY_LIST;
	*l = empty;
}
/*****************************************************************************/
static inline int lib_rflow_alloc_list(size_t size, struct lib_rflow_list *l)
{
	struct lib_rflow_cycle *arr;
	arr = (struct lib_rflow_cycle *)malloc(
		size * sizeof(*arr)
	);
	if(arr == NULL) {
		return 1;
	}

	l->cycles     = arr;
	l->num_cycles = 0;
	l->mem_size   = size;

	return 0;
}
/*****************************************************************************/
static inline void lib_rflow_free_list(struct lib_rflow_list *l)
{
	free(l->cycles);
}
/*****************************************************************************/
static inline double lib_rflow_matrix_mean_max(
	const struct lib_rflow_matrix *m
)
{
	double mean_max =   m->mean_min
	                  + (double)m->mean_bin_count * m->mean_bin_size;
	return mean_max;
}
/*****************************************************************************/
static inline double lib_rflow_matrix_amp_max(const struct lib_rflow_matrix *m)
{
	double amp_max  =   m->amp_min
	                  + (double)m->amp_bin_count * m->amp_bin_size;
	return amp_max;
}
/*****************************************************************************/
static inline unsigned* lib_rflow_bin_ptr(const struct lib_rflow_matrix *m,
                                          double amp, double mean)
{
	double mean_max = lib_rflow_matrix_mean_max(m);
	double amp_max  = lib_rflow_matrix_amp_max(m);
	size_t amp_bin;
	size_t mean_bin;

	if(mean >= mean_max || mean < m->mean_min) {
		return NULL;
	} else if(amp >= amp_max || amp < m->amp_min) {
		return NULL;
	}

	mean_bin = (size_t)((mean - m->mean_min) / m->mean_bin_size);
	amp_bin  = (size_t)((amp  - m->amp_min)  / m->amp_bin_size);

	return &m->bins[mean_bin + m->mean_bin_count * amp_bin];
}
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* LIB_RFLOW_H_ */
