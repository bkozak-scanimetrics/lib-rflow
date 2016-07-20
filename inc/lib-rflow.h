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
/******************************************************************************
*                                   DEFINES                                   *
******************************************************************************/
#if __GNUC__
#define EXPORT __attribute__((visibility("default")))
#else
#error "compiler not supported"
#endif
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
struct lib_rflow_init {
	int amp_bin_count;
	int mean_bin_count;

	double mean_min;
	double amp_min;
	double mean_bin_size;
	double amp_bin_size;
};
/*****************************************************************************/
struct rf_matrix {
	int      amp_bin_count;
	int      mean_bin_count;
	double   mean_min;
	double   amp_min;
	double   mean_bin_size;
	double   amp_bin_size;

	unsigned *bins;
};
/*****************************************************************************/
struct lib_rflow_cycle {
	double cycle_start;
	double cycle_end;
};
/*****************************************************************************/
struct lib_rflow_state;
/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/
EXPORT
struct lib_rflow_state* lib_rflow_init(const struct lib_rflow_init *init);
EXPORT
int lib_rflow_count(struct lib_rflow_state *s, const double *arr, size_t num);
EXPORT
struct rf_matrix* lib_rflow_get_matrix(struct lib_rflow_state *s);
EXPORT
void lib_rflow_destroy(struct lib_rflow_state *s);
EXPORT
size_t lib_rflow_string_matrix(const struct rf_matrix *m, char **cstr_out);
/******************************************************************************
*                              STATIC FUNCTIONS                               *
******************************************************************************/
static inline double lib_rflow_matrix_mean_max(const struct rf_matrix *m)
{
	double mean_max =   m->mean_min
	                  + (double)m->mean_bin_count * m->mean_bin_size;
	return mean_max;
}
/*****************************************************************************/
static inline double lib_rflow_matrix_amp_max(const struct rf_matrix *m)
{
	double amp_max  =   m->amp_min
	                  + (double)m->amp_bin_count * m->amp_bin_size;
	return amp_max;
}
/*****************************************************************************/
static inline unsigned* lib_rflow_bin_ptr(const struct rf_matrix *m,
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
