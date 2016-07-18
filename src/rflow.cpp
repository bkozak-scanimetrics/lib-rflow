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
#include "private/rflow.h"

#include "lib-rflow.h"
#include "private/rf-cycle.h"
/******************************************************************************
*                               PRIVATE METHODS                               *
******************************************************************************/
void rf_state::add_compressive(double p)
{
	compressive_cycles.emplace_back(p, CYCLE_COMPRESSIVE);
}
/*****************************************************************************/
void rf_state::add_tensile(double p)
{
	tensile_cycles.emplace_back(p, CYCLE_TENSILE);
}
/*****************************************************************************/
void rf_state::look_for_pv(double p)
{
	switch(cycle_state)
	{
	case INIT_0:
		cycle_state = INIT_1;
		break;
	case INIT_1:
		if(last_point > p) {
			cycle_state = HAVE_VALLEY;
			add_tensile(last_point);
		} else if(last_point < p) {
			cycle_state = HAVE_PEAK;
			add_compressive(last_point);
		}
		break;
	case HAVE_PEAK:
		if(last_point > p) {
			cycle_state = HAVE_VALLEY;
			add_tensile(last_point);
		}
		break;
	case HAVE_VALLEY:
		if(last_point < p) {
			cycle_state = HAVE_PEAK;
			add_compressive(last_point);
		}
		break;
	}
}
/*****************************************************************************/
void rf_state::continue_cycles(double p)
{
	std::list<rf_cycle> *l = nullptr;
	if(cycle_state == HAVE_PEAK) {
		l = &compressive_cycles;
	} else if(cycle_state == HAVE_VALLEY) {
		l = &tensile_cycles;
	} else {
		return;
	}

	auto i = l->begin();
	while(i != l->end()) {
		bool terminated = i->extend(p);
		if(terminated) {
			count_finished_cycle(*i);
			i = l->erase(i);
		} else {
			i++;
		}
	}
}
/*****************************************************************************/
void rf_state::do_merges(void)
{
	std::list<rf_cycle> *l = nullptr;
	if(cycle_state == HAVE_PEAK) {
		l = &compressive_cycles;
	} else if(cycle_state == HAVE_VALLEY) {
		l = &tensile_cycles;
	} else {
		return;
	}

	/* TODO - perform merging */
}
/*****************************************************************************/
void rf_state::process_point(double p)
{
	look_for_pv(p);
	continue_cycles(p);
	do_merges();
	last_point = p;
}
/*****************************************************************************/
void rf_state::count_finished_cycle(const rf_cycle &c)
{
	/* TODO - implement counting cycles */
}
/******************************************************************************
*                               PUBLIC METHODS                                *
******************************************************************************/
rf_state::rf_state(struct rf_matrix *matrix)
: matrix{matrix}, compressive_cycles{}, tensile_cycles{}
{
	cycle_state = INIT_0;
}
/*****************************************************************************/
void rf_state::count(const double *points, size_t num)
{
	for(size_t i = 0; i < num; i++) {
		process_point(points[i]);
	}
}
/*****************************************************************************/
struct rf_matrix* rf_state::get_matrix(void)
{
	return matrix;
}
/*****************************************************************************/