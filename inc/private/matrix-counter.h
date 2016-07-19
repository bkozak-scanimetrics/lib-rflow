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
#ifndef MATRIX_COUNTER_H_
#define MATRIX_COUNTER_H_
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include "lib-rflow.h"
#include "private/cycle-processor.h"
#include "private/rf-cycle.h"
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
class matrix_counter : public cycle_processor {
private:
	struct rf_matrix *const matrix;
public:
	matrix_counter(struct rf_matrix *m);

	void proc_cycle(const rf_cycle &c);
	void end_history(void);

	const struct rf_matrix *get_matrix(void) const;
};
/*****************************************************************************/
#endif /* MATRIX_COUNTER_H_ */