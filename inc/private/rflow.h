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
#ifndef RFLOW_H_
#define RFLOW_H_
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include "lib-rflow.h"

#include "private/rf-cycle.h"

#include <stdlib.h>

#include <list>
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
struct rf_state {
private:
	struct rf_matrix *const matrix;

	std::list<rf_cycle>compressive_cycles;
	std::list<rf_cycle>tensile_cycles;

	double last_point;
	double current_pv;
	enum cycle_state {INIT_0, INIT_1, HAVE_PEAK, HAVE_VALLEY} cycle_state;

	void add_new_cycle(void);
	void set_transition(enum cycle_state new_state, double pv);
	bool peak_valley_transition(double p);
	void process_point(double p);
	void add_compressive(double p);
	void add_tensile(double p);
	void flow_newest(void);
	void process_opposite_points(void);
	void do_merges(void);
	void count_finished_cycle(const rf_cycle &c);
	void clear_cycles(std::list<rf_cycle> *list);
public:
	rf_state(struct rf_matrix *matrix);
	~rf_state(void);
	void terminate(void);
	void count(const double *points, size_t num);
	struct rf_matrix* get_matrix(void);
};
/*****************************************************************************/
#endif /* RFLOW_H_ */