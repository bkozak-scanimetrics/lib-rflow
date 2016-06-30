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

#include <stdlib.h>
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
struct rf_state {
private:
	struct rf_matrix *const matrix;
public:
	rf_state(struct rf_matrix *matrix);
	void count(const double *points, size_t num);
	struct rf_matrix* get_matrix(void);
};
/*****************************************************************************/
#endif /* RFLOW_H_ */