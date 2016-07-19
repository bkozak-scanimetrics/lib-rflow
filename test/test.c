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

#include <stdlib.h>
#include <stdio.h>
/******************************************************************************
*                                   DEFINES                                   *
******************************************************************************/
#define ARR_SIZE(a) (sizeof(a)/sizeof(a[0]))
/******************************************************************************
*                                    DATA                                     *
******************************************************************************/
static const double test_data_0[] = {
	-2.0, 1.0,
	-3.0, 5.0,
	-1.0, 3.0,
	-4.0, 4.0,
	-2.0
};
/* http://citeseerx.ist.psu.edu/viewdoc/download
   ?doi=10.1.1.444.3640&rep=rep1&type=pdf */
/*****************************************************************************/
static const double test_data_1[] = {
	1.1, -4.6, -0.2, -3.5, -0.8, -2.7, 6.0
}; /* http://nal-ir.nal.res.in/9082/1/K.L_._SIngh_and_V.R.Ramganath.pdf */
/******************************************************************************
*                            FUNCTION DEFINITIONS                             *
******************************************************************************/
static void test_0(void)
{
	struct rf_state  *state;
	struct rf_matrix *matrix;
	char *string_matrix;
	struct rf_init i = {
		.amp_bin_count  = 12,
		.mean_bin_count = 1,
		.mean_min       = -6.0,
		.amp_min        = 0.0,
		.mean_bin_size  = 12.0,
		.amp_bin_size   = 1.0
	};

	state = lib_rflow_init(&i);
	matrix = lib_rflow_get_matrix(state);

	lib_rflow_count(state, test_data_0, ARR_SIZE(test_data_0));

	lib_rflow_destroy(state);

	lib_rflow_string_matrix(matrix, &string_matrix);

	printf("%s", string_matrix);

	free(string_matrix);
	free(matrix);
}
/*****************************************************************************/
int main(int argc, char **argv)
{
	test_0();
	return 0;
}
/*****************************************************************************/