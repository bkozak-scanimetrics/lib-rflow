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

#include <assert.h>
/******************************************************************************
*                              STATIC FUNCTIONS                               *
******************************************************************************/
static std::list<rf_cycle>::iterator
find_merge_point(const std::list<rf_cycle>::iterator &start,
                 const std::list<rf_cycle>::iterator &end,
                 const rf_cycle &item)
{
	auto i = start;
	for(i++; i != end; i++) {
		if(item.needs_merge(*i)) {
			break;
		}
	}

	return i;
}
/******************************************************************************
*                               PRIVATE METHODS                               *
******************************************************************************/
void rf_state::clear_cycles(std::list<rf_cycle> *list)
{
	while(list->size()) {
		rf_cycle &c = list->front();
		c.terminate();
		count_finished_cycle(c);
		list->pop_front();
	}
}
/*****************************************************************************/
void rf_state::add_compressive(double p)
{
	compressive_cycles.emplace_front(p, CYCLE_COMPRESSIVE);
}
/*****************************************************************************/
void rf_state::add_tensile(double p)
{
	tensile_cycles.emplace_front(p, CYCLE_TENSILE);
}
/*****************************************************************************/
void rf_state::set_transition(enum cycle_state new_state, double pv)
{
	cycle_state = new_state;
	current_pv  = pv;
}
/*****************************************************************************/
bool rf_state::peak_valley_transition(double p)
{
	bool transition = false;
	switch(cycle_state)
	{
	case INIT_0:
		set_transition(INIT_1, p);
		/* we don't set transition = true here because we haven't
		   actually identified a peak or valley */
		break;
	case INIT_1:
		if(last_point > p) {
			set_transition(HAVE_VALLEY, last_point);
			transition = true;
		} else if(last_point < p) {
			set_transition(HAVE_PEAK, last_point);
			transition = true;
		}
		break;
	case HAVE_PEAK:
		if(last_point > p) {
			set_transition(HAVE_VALLEY, last_point);
			transition = true;
		}
		break;
	case HAVE_VALLEY:
		if(last_point < p) {
			set_transition(HAVE_PEAK, last_point);
			transition = true;
		}
		break;
	default:
		break;
	}
	last_point = p;
	return transition;
}
/*****************************************************************************/
void rf_state::add_new_cycle(void)
{

	if(cycle_state == HAVE_VALLEY) {
		add_tensile(current_pv);
	} else {
		add_compressive(current_pv);
	}
}
/*****************************************************************************/
void rf_state::set_flow_point(void)
{
	rf_cycle *unflowing;

	/* the newest cycle we've created should not have a flow point set yet.
	   We have to set one here before we can start to look for cycle
	   terminations, process merges etc. */
	/* Note also that new cycles are added at the front of the list */
	if(cycle_state == HAVE_PEAK) {
		unflowing = &tensile_cycles.front();
	} else {
		unflowing = &compressive_cycles.front();
	}

	assert(!unflowing->is_flowing());
	unflowing->set_flow_point(current_pv);
}
/*****************************************************************************/
void rf_state::process_opposite_points(void)
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
		bool terminated = i->process_opposite_point(current_pv);
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
	} else {
		l = &tensile_cycles;
	}

	auto i = l->begin();

	while(i != l->end()) {
		auto merge_point = find_merge_point(i, l->end(), *i);
		if(merge_point != l->end()) {
			i->merge(&*merge_point);
			count_finished_cycle(*i);
			i = l->erase(i);
		} else {
			/* TODO - should we actually just break out here?
			   I think that it shouldn't be possible for merges
			   to take place unless the cycle has just been added
			   or has just been merged into. */
			i++;
		}
	}
}
/*****************************************************************************/
void rf_state::process_point(double p)
{
	if(peak_valley_transition(p)) {
		assert(cycle_state == HAVE_VALLEY || cycle_state == HAVE_PEAK);
		add_new_cycle();
		set_flow_point();
		process_opposite_points();
		do_merges();
	}
}
/*****************************************************************************/
void rf_state::count_finished_cycle(const rf_cycle &c)
{
	unsigned *bin_ptr = lib_rflow_bin_ptr(matrix, c.magnitude(), c.mean());

	if(bin_ptr != NULL) {
		*bin_ptr += 1;
	}
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
rf_state::~rf_state(void)
{
	terminate();
}
/*****************************************************************************/
void rf_state::terminate(void)
{
	switch(cycle_state)
	{
	case INIT_0:
		return;
	case INIT_1:
		cycle_state = INIT_0;
		return;
	case HAVE_PEAK:
		set_transition(HAVE_VALLEY, last_point);
		break;
	case HAVE_VALLEY:
		set_transition(HAVE_PEAK, last_point);
		break;
	}

	set_flow_point();
	process_opposite_points();
	do_merges();

	clear_cycles(&compressive_cycles);
	clear_cycles(&tensile_cycles);

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