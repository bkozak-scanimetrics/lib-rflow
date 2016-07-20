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
#ifndef CUSTOM_CYCLE_PROC_H_
#define CUSTOM_CYCLE_PROC_H_
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include "private/cycle-processor.h"

#include "lib-rflow.h"
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
class custom_cycle_proc : public cycle_processor {
public:
	typedef void (*processor)(const struct lib_rflow_cycle *, void*);
	typedef void (*finisher)(void*);

	custom_cycle_proc(processor func, finisher fini, void *state);
	~custom_cycle_proc(void);
	void proc_cycle(const rf_cycle &c);
	void end_history(void);
private:
	const processor proc;
	const finisher  fini;
	void *const state;
};
/*****************************************************************************/
#endif /* CUSTOM_CYCLE_PROC_H_ */