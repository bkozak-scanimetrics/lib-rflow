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
/*****************************************************************************/
static const double test_simple_merge[] = {
	1.0, -2.0,
	0.0, -3.0,
	2.0
};
/******************************************************************************
*                            FUNCTION DEFINITIONS                             *
******************************************************************************/
static void test_0(void)
{
	struct lib_rflow_state  *state;
	struct lib_rflow_cycle *cycles;
	size_t                 num_cycles;
	struct lib_rflow_init i = {
		.opts           = LIB_RFLOW_MODE_PASSTHROUGH
	};

	state = lib_rflow_init(&i);

	lib_rflow_count(state, test_data_0, ARR_SIZE(test_data_0));
	lib_rflow_end_history(state);

	num_cycles = lib_rflow_pop_cycles(state, &cycles);
	for(size_t i = 0; i < num_cycles; i++) {
		printf(
			"cycle %d %lf -> %lf\n",
			i, cycles[i].cycle_start, cycles[i].cycle_end
		);
	}

	lib_rflow_destroy(state);
	free(cycles);
}
/*****************************************************************************/
int main(int argc, char **argv)
{
	test_0();
	return 0;
}
/*****************************************************************************/