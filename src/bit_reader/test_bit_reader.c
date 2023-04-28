#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "miniunit.h"

static const char* _write_file(char const* path, uint8_t const* bytes, size_t num_bytes) {
	FILE* file = fopen(path, "w");
	fwrite(bytes, sizeof(*bytes), num_bytes, file);
	fclose(file);

	return path; // Makes writing test cases more compact
}

static int _test_simple() {
	mu_start();

	// create file to read from
	uint8_t bytes[] = { 0xaa }; // 1010 1010
	char const* path = _write_file("test_simple.txt", bytes, sizeof(bytes));

	// read bits from file
	BitReader reader = open_bit_reader(path);
	read_bits(&reader, 3); // Should read 101
	
	// ensure correctness
	mu_check(reader.num_bits_read == 3);
	mu_check(reader.current_byte  == 0xaa);


	mu_end();

}

int main(int argc, char* argv[]) {
	mu_run(_test_simple);
	
	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
