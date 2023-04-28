#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "bit_reader.h"
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

	BitReader reader = open_bit_reader(path);
	mu_check(reader.num_bits_left == 8);
	mu_check(reader.current_byte  == 0x00aa);

	read_bits(&reader, 3); // Should read 101
	
	mu_check(reader.num_bits_left == 5);
	mu_check(reader.current_byte  == 0x00aa);
	mu_check(reader_tell(reader) == 0x05);

	close_bit_reader(&reader);

	mu_end();
}

static int _test_exact() {
	mu_start();

	uint8_t bytes[] = { 0xaa, 0x55 }; // 1010 1010
	char const* path = _write_file("test_exact.txt", bytes, sizeof(bytes));

	BitReader reader = open_bit_reader(path);
	mu_check(reader.num_bits_left == 8);
	mu_check(reader.current_byte  == 0x00aa);

	read_bits(&reader, 8); // Should read 1010 1010
	
	mu_check(reader.num_bits_left == 8);
	mu_check(reader.current_byte  == 0xaa55);
	printf("0x%x\n", reader_tell(reader));
	mu_check(reader_tell(reader) == 0xaa);

	close_bit_reader(&reader);

	mu_end();
}

static int _test_overflow() {
	mu_start();

	uint8_t bytes[] = { 0xaa, 0x55, 0xbb }; // 1010 1010
	char const* path = _write_file("test_overflow.txt", bytes, sizeof(bytes));

	BitReader reader = open_bit_reader(path);
	mu_check(reader.current_byte  == 0x00aa);
	mu_check(reader.num_bits_left == 8);
	mu_check(reader_tell(reader) == 0x00);

	read_bits(&reader, 5); // Should read 1010 1
	mu_check(reader.current_byte  == 0x00aa);
	mu_check(reader.num_bits_left == 3);
	printf("here 0x%x\n", reader_tell(reader));
	mu_check(reader_tell(reader) == 0x15);

	read_bits(&reader, 8); // should read       010  0101 0
	mu_check(reader.current_byte  == 0xaa55);
	mu_check(reader.num_bits_left == 3);
	mu_check(reader_tell(reader) == 0x4a);
	
	printf("0x%x\n", reader_tell(reader));

	close_bit_reader(&reader);

	mu_end();
}

static int _test_feof() {
	mu_start();

	uint8_t bytes[] = { 0xaa }; // 1010 1010
	char const* path = _write_file("test_feof.txt", bytes, sizeof(bytes));

	BitReader reader = open_bit_reader(path);
	mu_check(reader.current_byte  == 0x00aa);
	mu_check(reader.num_bits_left == 8);
	mu_check(reader_tell(reader) == 0x00);

	read_bits(&reader, 8); // should read 1010 1010
	mu_check(reader.current_byte  == 0xaa00);
	mu_check(reader.num_bits_left == 8);
	mu_check(reader_tell(reader) == 0xaa);

	read_bits(&reader, 8); // should read 0000 0000
	printf("here 0x%x\n", reader_tell(reader));
	mu_check(reader.current_byte  == 0x0000);
	mu_check(reader.num_bits_left == 8);
	mu_check(reader_tell(reader) == 0x00);
	
	read_bits(&reader, 8); // should read 0000 0000
	mu_check(reader.current_byte  == 0x0000);
	mu_check(reader.num_bits_left == 8);
	mu_check(reader_tell(reader) == 0x00);

	close_bit_reader(&reader);

	mu_end();
}

int main(int argc, char* argv[]) {
	mu_run(_test_simple);
	mu_run(_test_exact);
	mu_run(_test_overflow);
	mu_run(_test_feof);
	
	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
