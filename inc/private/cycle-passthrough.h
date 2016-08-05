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
#ifndef CYCLE_PASSTHROUGH_H_
#define CYCLE_PASSTHROUGH_H_
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include "private/cycle-processor.h"
#include "lib-rflow.h"
/******************************************************************************
*                                    TYPES                                    *
******************************************************************************/
class cycle_passthrough : public cycle_processor {
private:
	const size_t default_alloc;
	bool history_ended;
	struct lib_rflow_list cycle_list;
public:
	cycle_passthrough(void);
	cycle_passthrough(size_t default_alloc);
	~cycle_passthrough(void);

	void proc_cycle(const rf_cycle &c);
	void end_history(void);

	struct lib_rflow_list pop_cycle_list(void);
	struct lib_rflow_list pop_cycle_list(
		const struct lib_rflow_list *new_mem
	);

	size_t list_size(void) const;
};
/*****************************************************************************/
#endif /* CYCLE_PASSTHROUGH_H_ */