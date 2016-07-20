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
#include "private/cycle-passthrough.h"

#include <new>

#include <assert.h>
#include <stdlib.h>
/******************************************************************************
*                              STATIC FUNCTIONS                               *
******************************************************************************/
static struct lib_rflow_cycle *alloc_cycles(size_t size)
{
	struct lib_rflow_cycle *arr = (struct lib_rflow_cycle *)malloc(
		size * sizeof(*arr)
	);
	if(arr == NULL) {
		throw std::bad_alloc{};
	}
	return arr;
}
/*****************************************************************************/
static struct lib_rflow_cycle *realloc_cycles(
	size_t new_size, struct lib_rflow_cycle *p
) {
	void *new_mem = realloc((void*)p, new_size);
	if(new_mem == NULL) {
		throw std::bad_alloc{};
	}
	return (struct lib_rflow_cycle *)new_mem;
}
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
size_t cycle_passthrough::pop_cycle_list(struct lib_rflow_cycle **p,
                                         struct lib_rflow_cycle *new_mem,
                                         size_t new_mem_size)
{
	size_t tmp_cycle_count = cycle_count;

	if(cycle_count == 0) {
		return 0;
	}

	*p     = cycles;

	if(history_ended) {
		cycles   = NULL;
		arr_size = 0;
	} else if(new_mem == NULL) {
		cycles   = alloc_cycles(default_alloc);
		arr_size = default_alloc;
	} else {
		assert(new_mem_size > 0);
		cycles = new_mem;
		arr_size = new_mem_size;
	}

	arr_size = 0;

	return tmp_cycle_count;
}
/*****************************************************************************/
size_t cycle_passthrough::pop_cycle_list(struct lib_rflow_cycle **p)
{
	return pop_cycle_list(p, NULL, 0);
}
/*****************************************************************************/
void cycle_passthrough::end_history(void)
{
	history_ended = true;
}
/*****************************************************************************/
void cycle_passthrough::proc_cycle(const rf_cycle &c)
{
	assert(!history_ended);
	size_t new_count = cycle_count + 1;
	if(new_count > arr_size) {
		size_t new_size = arr_size * 2;
		cycles    = realloc_cycles(new_size, cycles);
		arr_size  = new_size;
	}

	cycles[cycle_count] = {c.get_cycle_start(), c.get_cycle_end()};

	cycle_count = new_count;
}
/*****************************************************************************/
cycle_passthrough::~cycle_passthrough(void)
{
	free(cycles);
}
/*****************************************************************************/
cycle_passthrough::cycle_passthrough(size_t default_alloc)
: default_alloc{default_alloc}, cycle_count{0}, arr_size{default_alloc},
  cycles{alloc_cycles(default_alloc)}, history_ended{false}
{

}
/*****************************************************************************/
cycle_passthrough::cycle_passthrough(void)
: cycle_passthrough(1024)
{
}
/*****************************************************************************/