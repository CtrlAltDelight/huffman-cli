# VARIABLES

# Define directories
C_DIR=src
O_DIR=obj
H_DIR=include

# Define file names
SRC_C=$(filter-out $(wildcard src/**/test*.c), $(wildcard src/**/*.c))
SRC_H=$(wildcard include/*.h)
OBJS=$(SRC_C:src/%.c=obj/%.o) # Same as SRC_C, but replace src/ at the beginning with obj/ and replace .c with .o
TEST_C=test_$(BASE_NAME).c
EXECUTABLE=test_$(BASE_NAME)
EXECUTABLE_GCOV=$(EXECUTABLE)_gcov

# Define compiler options
CC=gcc
CFLAGS=-g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic -I include
CFLAGS_GCOV=$(CFLAGS) -fprofile-arcs -ftest-coverage

SHELL=/bin/bash

# RULES


print: 
	@echo "SRC_C:"
	@echo $(SRC_C)
	@echo "SRC_H:"
	@echo $(SRC_H)
	@echo "OBJS:"
	@echo $(OBJS)

obj/%.o: src/%.c # Any .o file with prerequisite of itself.c (EX: make huffman/huffman.o)
	$(CC) $(CFLAGS) -c $< -o $@ # $< is the first prerequisite (EX: huffman.c), $@ is the target file (EX: huffman.o)

$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) $(SRC_C) $(TEST_C) $(CFLAGS) -o $(EXECUTABLE)

prog: $(OBJS)
	$(CC) src/main.c $(CFLAGS) $^ -o $@

clean:
	rm -f *.c.gcov *.gcda *.gcno $(EXECUTABLE) $(EXECUTABLE_GCOV)

.PHONY: test coverage clean print
