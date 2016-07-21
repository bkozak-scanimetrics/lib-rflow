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
#include "test.h"

#include "lib-rflow.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/******************************************************************************
*                                   DEFINES                                   *
******************************************************************************/
#define ARR_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define ABS(i) ((i) < 0 ? -(i) : (i))
/******************************************************************************
*                                    DATA                                     *
******************************************************************************/
static const double test0_data[] = {
	-2.0, 1.0,
	-3.0, 5.0,
	-1.0, 3.0,
	-4.0, 4.0,
	-2.0
};
/* http://citeseerx.ist.psu.edu/viewdoc/download
   ?doi=10.1.1.444.3640&rep=rep1&type=pdf */
static const struct lib_rflow_cycle test0_expected[] = {
	{-2.0,  1.0},
	{ 1.0, -3.0},
	{-1.0,  3.0},
	{-3.0,  5.0},
	{ 3.0, -1.0},
	{ 4.0, -2.0},
	{ 5.0, -4.0},
	{ -4.0, 4.0}
};
/*****************************************************************************/
static const double test1_data[] = {
	1.1, -4.6, -0.2, -3.5, -0.8, -2.7, 6.0
}; /* http://nal-ir.nal.res.in/9082/1/K.L_._SIngh_and_V.R.Ramganath.pdf */

static const struct lib_rflow_cycle test1_expected[] = {
	{-0.8, -2.7},
	{-0.2, -3.5},
	{ 1.1, -4.6},
	{-2.7, -0.8},
	{-3.5, -0.2},
	{-4.6,  6.0}
};
/*****************************************************************************/
static const double test2_data[] = {
	 1.0, -3.0,
	-1.0, -4.0,
	 0.0, -5.0,
	 0.0
};
static const struct lib_rflow_cycle test2_expected[] = {
	{-3.0, -1.0},
	{-1.0, -3.0},
	{-4.0,  0.0},
	{ 0.0, -4.0},
	{ 1.0, -5.0},
	{-5.0,  0.0}
};
/*****************************************************************************/
static const double test3_data[] = {
	1.0, -2.0,
	0.0, -3.0,
	2.0
};
static const struct lib_rflow_cycle test3_expected[] = {
	{-2.0,  0.0},
	{ 0.0, -2.0},
	{ 1.0, -3.0},
	{-3.0,  2.0}
};
/*****************************************************************************/
static const double test4_data[] = {
	2.0, -2.0,
	1.0, -1.0,
	0.0, -4.0,
	3.0
};
static const struct lib_rflow_cycle test4_expected[] = {
	{-1.0,  0.0},
	{-2.0,  1.0},
	{ 0.0, -1.0},
	{ 1.0, -2.0},
	{ 2.0, -4.0},
	{-4.0,  3.0}
};
/******************************************************************************
*                                   MACROS                                    *
******************************************************************************/
#define ASSERT_CYCLES_EXPECTED(count, cycles, expected) \
	assert_cycles_equal(count, cycles, ARR_SIZE(expected), expected)

#define TEST_CYCLES_EXPECTED(arr_data, arr_expected) \
do{ \
	struct lib_rflow_state  *state; \
	struct lib_rflow_cycle *cycles; \
	size_t                 num_cycles; \
	struct lib_rflow_init i = { \
		.opts           = LIB_RFLOW_MODE_PASSTHROUGH \
	}; \
	state = lib_rflow_init(&i); \
	lib_rflow_count(state, arr_data, ARR_SIZE(arr_data)); \
	lib_rflow_end_history(state); \
	num_cycles = lib_rflow_pop_cycles(state, &cycles); \
	ASSERT_CYCLES_EXPECTED(num_cycles, cycles, arr_expected); \
	lib_rflow_destroy(state); \
	free(cycles); \
} while(0)
/******************************************************************************
*                            FUNCTION DEFINITIONS                             *
******************************************************************************/
static void assert_cycles_equal(
	size_t n0, const struct lib_rflow_cycle *c0,
	size_t n1, const struct lib_rflow_cycle *c1
)
{
	TEST_ASSERT(n0 == n1);
	for(size_t n = 0; n < n0; n++) {
		TEST_ASSERT(c0[n].cycle_start == c1[n].cycle_start);
		TEST_ASSERT(c0[n].cycle_end   == c1[n].cycle_end  );
	}
}
/*****************************************************************************/
static void test_0(void)
{
	TEST_CYCLES_EXPECTED(test0_data, test0_expected);
}
/*****************************************************************************/
static void test_1(void)
{
	TEST_CYCLES_EXPECTED(test1_data, test1_expected);
}
/*****************************************************************************/
static void test_2(void)
{
	TEST_CYCLES_EXPECTED(test2_data, test2_expected);
}
/*****************************************************************************/
static void test_3(void)
{
	TEST_CYCLES_EXPECTED(test3_data, test3_expected);
}
/*****************************************************************************/
static void test_4(void)
{
	TEST_CYCLES_EXPECTED(test4_data, test4_expected);
}
/*****************************************************************************/
int main(int argc, char **argv)
{
	TEST_BLOCK_START();
	RUN_TEST(test_0);
	RUN_TEST(test_1);
	RUN_TEST(test_2);
	RUN_TEST(test_3);
	RUN_TEST(test_4);
	TEST_BLOCK_END();
}
/*****************************************************************************/