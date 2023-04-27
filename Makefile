# VARIABLES

# Define file names
MAIN_C=main.c
SRC_C=$(filter-out $(wildcard src/**/test*.c), $(wildcard src/**/*.c))
SRC_H=$(wildcard include/*.h)
OBJS=$(SRC_C:src/%.c=obj/%.o) $(MAIN_C:.c=.o) # Same as SRC_C, but replace src/ at the beginning with obj/ and replace .c with .o
EXECUTABLE=bin/huffmancli
EXECUTABLE_GCOV=$(EXECUTABLE)_gcov

# Define compiler options
CC=gcc
CFLAGS=-g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic -I include
CFLAGS_GCOV=$(CFLAGS) -fprofile-arcs -ftest-coverage

SHELL=/bin/bash

YELLOW=\033[1;33m
CLEAR=\033[0m

# RULES


print: 
	@echo "SRC_C:"
	@echo $(SRC_C)
	@echo "SRC_H:"
	@echo $(SRC_H)
	@echo "OBJS:"
	@echo $(OBJS)

obj/%.o: src/%.c # Any .o file with prerequisite of itself.c (EX: make huffman/huffman.o)
	@echo -e "$(YELLOW)Building $@ from $<...$(CLEAR)"
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ # $< is the first prerequisite (EX: huffman.c), $@ is the target file (EX: huffman.o)

$(EXECUTABLE): $(OBJS)
	@echo -e "$(YELLOW)Building $@ from $^...$(CLEAR)"
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

run: $(EXECUTABLE)
	@echo -e "$(YELLOW)Running $<...$(CLEAR)"
	@$(EXECUTABLE)

clean:
	rm -rf obj bin *.c.gcov *.gcda *.gcno $(EXECUTABLE) $(EXECUTABLE_GCOV)

.PHONY: test coverage clean print
