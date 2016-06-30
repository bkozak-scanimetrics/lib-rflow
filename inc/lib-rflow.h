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
*                                    TYPES                                    *
******************************************************************************/
struct rf_matrix {
	int      amp_bin_count;
	int      mean_bin_count;
	double   mean_min;
	double   amp_min;

	unsigned *bins;
};
/*****************************************************************************/
struct rf_state;
/******************************************************************************
*                             FUNCTION PROTOTYPES                             *
******************************************************************************/
struct rf_state* lib_rflow_init(int amp_bin_count, int mean_bin_count);
int lib_rflow_count(struct rf_state *state, const double *points, size_t num);
struct rf_matrix* lib_rflow_get_matrix(struct rf_state *state);
int lib_rflow_destroy(struct rf_state *state);
/*****************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* LIB_RFLOW_H_ */