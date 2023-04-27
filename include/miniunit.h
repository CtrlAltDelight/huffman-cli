#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef __MINIUNIT_H__
#define __MINIUNIT_H__

#define MINIUNIT_BONUS_SELECTIVE_TESTING
#define MINIUNIT_BONUS_INLINE_FUNCTIONS

#define MU_SUCCESS 0

#define mu_start() int __mu_first_failure_line_number_or_0 = MU_SUCCESS

inline static void _mu_check(bool condition, int* a__mu_first_failure_line_number_or_0, int line_num) {
	if(*a__mu_first_failure_line_number_or_0 == 0 && !(condition)) {
		*a__mu_first_failure_line_number_or_0 = line_num;
	}
}

#define mu_check(condition) _mu_check((condition), &__mu_first_failure_line_number_or_0, __LINE__)

#define mu_check_strings_equal(s1, s2) mu_check(strcmp((s1), (s2)) == 0)

#define mu_end() return __mu_first_failure_line_number_or_0

inline static void _mu_run(char* function_name, int (*function)(void), int argc, char** argv) {
	bool is_specified = false;
	if(argc <= 1) { // argc > 1 because argc accounts for the name of the executable being run.
		is_specified = true;
	}
	for(; is_specified == false && argc > 1; argc--) { // allows for whitelisting as many functions as you want
		if(strcmp(argv[argc - 1], function_name) == 0) {
			is_specified = true;
		}
	}
	if(!is_specified) {
		return;
	}
	int line_failure_number = (*function)();
	if(line_failure_number == MU_SUCCESS) {
		printf("\x1b[32mTest passed: %s\x1b[0m\n", function_name);
	}
	else {
		printf("\x1b[31mTest failed: %s at line %d\x1b[0m\n", function_name, line_failure_number);
	}
}

#define mu_run(function) _mu_run((#function), (function), argc, argv)

#endif /* end of include guard: __MINIUNIT_H__ */
