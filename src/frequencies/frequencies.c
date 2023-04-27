#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "frequencies.h"

bool calc_frequencies(Frequencies freqs, const char* path, const char** a_error) {
	FILE* stream = fopen(path, "r");
	if(stream == NULL) {
		*a_error = strerror(errno);
		return false;
	}

	for(uint8_t character = fgetc(stream); !feof(stream); character = fgetc(stream)) {
		freqs[character] += 1;
	}
	fclose(stream);
	return true;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
