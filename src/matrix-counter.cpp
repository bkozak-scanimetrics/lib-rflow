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
#include "private/matrix-counter.h"

#include "lib-rflow.h"
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
void matrix_counter::end_history(void)
{
}
/*****************************************************************************/
const struct lib_rflow_matrix * matrix_counter::get_matrix(void) const
{
	return matrix;
}
/*****************************************************************************/
void matrix_counter::proc_cycle(const rf_cycle &c)
{
	unsigned *bin_ptr = lib_rflow_bin_ptr(matrix, c.magnitude(), c.mean());

	if(bin_ptr != NULL) {
		*bin_ptr += 1;
	}
}
/*****************************************************************************/
matrix_counter::matrix_counter(struct lib_rflow_matrix *m)
: matrix{m}
{

}
/*****************************************************************************/
