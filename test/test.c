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
/******************************************************************************
*                            FUNCTION DEFINITIONS                             *
******************************************************************************/
static void test_0(void)
{
	struct rf_state  *state;
	struct rf_matrix *matrix;
	struct rf_init i = {
		.amp_bin_count  = 128,
		.mean_bin_count = 128,
		.mean_min       = 0.0,
		.amp_min        = 0.0,
		.mean_bin_size  = 1.0,
		.amp_bin_size   = 1.0
	};

	state = lib_rflow_init(&i);
	matrix = lib_rflow_get_matrix(state);

	lib_rflow_destroy(state);
	free(matrix);
}
/*****************************************************************************/
int main(int argc, char **argv)
{
	test_0();
	return 0;
}
/*****************************************************************************/