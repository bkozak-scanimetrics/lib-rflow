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
#include "private/custom-cycle-proc.h"
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
void custom_cycle_proc::end_history(void)
{
	fini(state);
}
/*****************************************************************************/
void custom_cycle_proc::proc_cycle(const rf_cycle &c)
{
	struct lib_rflow_cycle cycle{c.get_cycle_start(), c.get_cycle_end()};

	proc(&cycle, state);
}
/*****************************************************************************/
custom_cycle_proc::~custom_cycle_proc(void)
{
}
/*****************************************************************************/
custom_cycle_proc::
custom_cycle_proc(processor func, finisher fini, void *state)
: proc{proc}, fini{fini}, state{state}
{
}
/*****************************************************************************/