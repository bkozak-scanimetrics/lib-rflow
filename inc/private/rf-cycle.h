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
#ifndef CYCLE_H_
#define CYCLE_H_

/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
enum cycle_direction{CYCLE_COMPRESSIVE, CYCLE_TENSILE};

class rf_cycle {
private:
	const double cycle_start;
	const enum cycle_direction direction;

	bool    terminated;
	bool    flowing;
	double  cycle_end;
	int     merge_count;
public:
	rf_cycle(double cycle_start, enum cycle_direction dir);

	void terminate(void);
	void merge(rf_cycle *other);
	void set_flow_point(double p);
	bool process_opposite_point(double p);

	bool needs_merge(const rf_cycle &that) const;
	double magnitude(void) const;
	double mean(void) const;

	bool is_terminated(void) const;
	bool is_flowing(void) const;
	double get_cycle_start(void) const;
	double get_cycle_end(void) const;
	int get_half_cycles(void) const;
};
/*****************************************************************************/
#endif /* CYCLE_H_ */