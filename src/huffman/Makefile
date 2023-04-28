#_____________________________________________________________
# VARIABLES
#
# CURRENT ASSIGNMENT
BASE_NAME=huffman
#
# EXECUTABLES
EXECUTABLE=test_$(BASE_NAME)
EXECUTABLE_GCOV=$(EXECUTABLE)_gcov
#
# SOURCE FILENAMES
TEST_C=$(EXECUTABLE).c
SRC_C=$(BASE_NAME).c
SRC_H=$(BASE_NAME).h
#
# SYSTEM
SHELL=/bin/bash
CC=gcc
CFLAGS=-g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic -Wno-unused-function -I ../../include
CFLAGS_GCOV=$(CFLAGS) -fprofile-arcs -ftest-coverage
#
# GCOV

#_____________________________________________________________
# RULES
#
$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) -o $(EXECUTABLE) $(SRC_C) $(TEST_C) $(CFLAGS) ../../obj/priority_queue/priority_queue.o ../../obj/bit_writer/bit_writer.o ../../obj/frequencies/frequencies.o

test: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(EXECUTABLE_GCOV) *.c.gcov *.gcda *.gcno 

coverage: $(SRC_C) $(TEST_C)
	$(CC) -o $(EXECUTABLE_GCOV) $(SRC_C) $(TEST_C) $(CFLAGS_GCOV)
	./$(EXECUTABLE_GCOV) &> /dev/null
	gcov -f $(SRC_C)

# .PHONY TARGET - declare targets that do not correspond directly to an output file
.PHONY: test clean coverage

# vim: set noexpandtab tabstop=4 filetype=make:
