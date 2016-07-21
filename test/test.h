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
#ifndef TEST_H_
#define TEST_H_
/******************************************************************************
*                                  INCLUDES                                   *
******************************************************************************/
#include <setjmp.h>
#include <stdio.h>
/******************************************************************************
*                                 TEST STATE                                  *
******************************************************************************/
static jmp_buf _test_return;
/******************************************************************************
*                                   DEFINES                                   *
******************************************************************************/
#define TEST_BLOCK_START() do {int    _test_block_err = 0
#define TEST_BLOCK_END()   return _test_block_err;}while(0)

#define TEST_PRINT(...)           fprintf(stderr, __VA_ARGS__ )
#define TEST_PRINT_RESULT(m, ...) TEST_PRINT("\t" m , ##__VA_ARGS__ )

#define RUN_TEST(t, ...) \
	do { \
		TEST_PRINT("Run test: '" #t "'\n"); \
		if(setjmp(_test_return)) { \
			_test_block_err = 1; \
			TEST_PRINT_RESULT("TEST FAILED\n"); \
		} else { \
			t(__VA_ARGS__); \
			TEST_PRINT_RESULT("SUCCESS\n"); \
		} \
	}while(0)

#define TEST_ASSERT(a) TEST_ASSERT_MSG(a, #a)
#define TEST_ASSERT_MSG(a, m) \
	do{ \
		if(!(a)){ \
			TEST_PRINT_RESULT("assert fail: '%s'\n", (m)); \
			longjmp(_test_return, 1); \
		}; \
	} while(0)
/*****************************************************************************/
#endif /* TEST_H_ */