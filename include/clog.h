#ifndef __CLOG_H__
#define __CLOG_H__

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[m"

#define __mu_log_color(color, stream, ...)	\
do {										\
	int fileno;								\
	if(stream == stdout) {					\
		fileno = STDOUT_FILENO;				\
	}										\
	else if(stream == stderr) {				\
		fileno = STDERR_FILENO;				\
	}										\
											\
	if(isatty(fileno)) {					\
		fprintf((stream), "%s", (color));	\
	}										\
	fprintf(stream, __VA_ARGS__);			\
	if(isatty(fileno)) {					\
		fprintf((stream), "%s", ANSI_RESET);\
	}										\
} while(false)

#ifdef DEBUG // enable
#  define log_msg(msg) 	   (fprintf(stdout, "%s\n", (msg)))
#  define log_int(n) 	   (fprintf(stdout, "%s == %d\n", (#n), (n)))
#  define log_str(s)       (fprintf(stdout, "%s == \"%s\"\n", (#s), (s)))
#  define log_char(ch)     (fprintf(stdout, "%s == '%c'\n", (#ch), (ch)))
#  define log_addr(addr)   (fprintf(stdout, "%s == %p\n", (#addr), (void*)(addr)))
#  define log_red(...)     __mu_log_color(ANSI_RED, stdout, __VA_ARGS__)
#  define log_green(...)   __mu_log_color(ANSI_GREEN, stdout, __VA_ARGS__)
#  define log_yellow(...)  __mu_log_color(ANSI_YELLOW, stdout, __VA_ARGS__)
#  define log_blue(...)    __mu_log_color(ANSI_BLUE, stdout, __VA_ARGS__)
#  define log_magenta(...) __mu_log_color(ANSI_MAGENTA, stdout, __VA_ARGS__)
#  define log_cyan(...)    __mu_log_color(ANSI_CYAN, stdout, __VA_ARGS__)

#else // disable
#  define log_msg(msg)
#  define log_int(n)
#  define log_str(s)
#  define log_char(c)
#  define log_addr(addr)
#  define log_red(...)
#  define log_green(...)
#  define log_yellow(...)
#  define log_blue(...)
#  define log_magenta(...)
#  define log_cyan(...)

#endif

#endif
