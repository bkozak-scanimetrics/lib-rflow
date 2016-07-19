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
#ifndef CYCLE_PROCESSOR_H_
#define CYCLE_PROCESSOR_H_
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include "private/rf-cycle.h"
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
class cycle_processor {
protected:
	cycle_processor(void);
public:
	virtual ~cycle_processor(void);
	virtual void proc_cycle(const rf_cycle &c) = 0;
	virtual void end_history(void) = 0;
};
/*****************************************************************************/
#endif /* CYCLE_PROCESSOR_H_ */