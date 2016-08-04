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
#include <string.h>
/******************************************************************************
*                              STATIC FUNCTIONS                               *
******************************************************************************/
static void realloc_cycles(size_t new_size, struct lib_rflow_list *l) {
	void *new_mem = realloc((void*)l->cycles, new_size);
	if(new_mem == NULL) {
		throw std::bad_alloc{};
	}
	l->mem_size = new_size;
	l->cycles   = (struct lib_rflow_cycle *)new_mem;
}
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
struct lib_rflow_list cycle_passthrough::pop_cycle_list(
	const struct lib_rflow_list *new_mem
)
{
	struct lib_rflow_list ret = cycle_list;

	if(history_ended) {
		ret.cycles = NULL;
	} else if(new_mem != NULL) {
		memcpy(&cycle_list, new_mem, sizeof(cycle_list));
	}

	return ret;
}
/*****************************************************************************/
struct lib_rflow_list cycle_passthrough::pop_cycle_list(void)
{
	struct lib_rflow_list new_mem;

	if(history_ended) {
		return pop_cycle_list(NULL);
	}

	if(lib_rflow_alloc_list(default_alloc, &new_mem)) {
		throw std::bad_alloc{};
	}

	return pop_cycle_list(&new_mem);
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

	size_t new_count = cycle_list.num_cycles + 1;
	if(new_count > cycle_list.mem_size) {
		size_t new_size = cycle_list.mem_size * 2;
		realloc_cycles(new_size, &cycle_list);
	}

	cycle_list.cycles[cycle_list.num_cycles] = {
		c.get_cycle_start(), c.get_cycle_end()
	};

	cycle_list.num_cycles = new_count;
}
/*****************************************************************************/
cycle_passthrough::~cycle_passthrough(void)
{
	lib_rflow_free_list(&cycle_list);
}
/*****************************************************************************/
cycle_passthrough::cycle_passthrough(size_t default_alloc)
: default_alloc{default_alloc}, history_ended{false}
{
	if(lib_rflow_alloc_list(default_alloc, &cycle_list)) {
		throw std::bad_alloc{};
	}
}
/*****************************************************************************/
cycle_passthrough::cycle_passthrough(void)
: cycle_passthrough(1024)
{
}
/*****************************************************************************/