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
#include "private/rf-cycle.h"

#include <assert.h>

#include <cmath>
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
rf_cycle::rf_cycle(double cycle_start, enum cycle_direction dir)
: cycle_start{cycle_start}, direction{dir}
{
	terminated  = false;
	cycle_end   = cycle_start;
	half_cycles = 1;
}
/*****************************************************************************/
void rf_cycle::terminate(void)
{
	terminated = true;
}
/*****************************************************************************/
void rf_cycle::merge(rf_cycle *that)
{
	assert(!this->terminated);
	assert(this->needs_merge(*that));

	that->terminate();
	this->half_cycles += that->half_cycles;
}
/*****************************************************************************/
bool rf_cycle::extend(double p)
{
	assert(!terminated);

	if(direction == TO_NEGATIVE) {
		if(p < cycle_end) {
			cycle_end = p;
		} else if(cycle_start < p) {
			terminate();
		}
	} else {
		if(p > cycle_end) {
			cycle_end = p;
		} else if(cycle_start > p) {
			terminate();
		}
	}

	return is_terminated();
}
/*****************************************************************************/
bool rf_cycle::needs_merge(const rf_cycle &that) const
{
	assert(!this->terminated);

	if(direction == TO_NEGATIVE) {
		return (
			this->cycle_start > that.cycle_start &&
			this->cycle_end <= that.cycle_end
		);
	} else {
		return (
			this->cycle_start < that.cycle_start &&
			this->cycle_end >= that.cycle_end
		);
	}
}
/*****************************************************************************/
double rf_cycle::magnitude(void) const
{
	return std::abs(cycle_start - cycle_end);
}
/*****************************************************************************/
double rf_cycle::mean(void) const
{
	return (cycle_start - cycle_end) / 2.0;
}
/*****************************************************************************/
int rf_cycle::get_half_cycles(void) const
{
	return half_cycles;
}
/*****************************************************************************/
bool rf_cycle::is_terminated(void) const
{
	return terminated;
}
/*****************************************************************************/
double rf_cycle::get_cycle_start(void) const
{
	return cycle_start;
}
/*****************************************************************************/
double rf_cycle::get_cycle_end(void) const
{
	return cycle_end;
}
/*****************************************************************************/