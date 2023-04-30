#include <assert.h>
#include "miniunit.h"
#include "bit_writer.h"

BitWriter open_bit_writer(const char* path) {
	return (BitWriter) { .file = fopen(path, "w"), .current_byte = 0, .num_bits_left = 8 };
}

void write_bits(BitWriter* a_writer, uint64_t bits, uint64_t num_bits_to_write) {
	// breaks down num_bits_to_write > 8 into separate calls to write_bits(...)
	if(num_bits_to_write > 8) {
		// first, write the leftmost bits that are not divisible by 8.
		uint64_t num_bits_to_shift = 8 * (num_bits_to_write / 8);
		write_bits(a_writer, bits >> num_bits_to_shift, num_bits_to_write - num_bits_to_shift);
		num_bits_to_write -= (num_bits_to_write - num_bits_to_shift);
		assert(num_bits_to_write % 8 == 0);

		// Now, write the rest of the bits, 8 at a time.
		for(; num_bits_to_write > 8; num_bits_to_write -= 8) {
			write_bits(a_writer, bits >> (num_bits_to_write - 8), 8);
		}
	}
	assert(0 <= num_bits_to_write && num_bits_to_write <= 8);
	assert(1 <= a_writer -> num_bits_left && a_writer -> num_bits_left <= 8);
	if(num_bits_to_write <= a_writer -> num_bits_left) { // Enough room
		// write them to .current_byte starting with MSB (left side)
		uint8_t bits_clean = bits;
		bits_clean = bits_clean & (0xff >> (8 - num_bits_to_write));
		bits_clean = bits_clean << (a_writer->num_bits_left - num_bits_to_write);
		a_writer -> current_byte |= bits_clean;
		a_writer -> num_bits_left -= num_bits_to_write; // update .num_bits_left with the number of unused bits remaining in .current_byte
	}
	else { // Not enough room
		uint8_t amt_bits_written = a_writer -> num_bits_left;
		write_bits(a_writer, bits >> (num_bits_to_write - amt_bits_written), amt_bits_written); // write as many as there is room for
		write_bits(a_writer, bits, num_bits_to_write - amt_bits_written); // write remaining bits from bits to .current_byte, starting with MSB (left side)
	}

	if(a_writer -> num_bits_left == 0) {
		fwrite(&((a_writer) -> current_byte), sizeof(a_writer -> current_byte), 1, a_writer -> file);
		a_writer -> current_byte = 0;
		a_writer -> num_bits_left = 8;
	}
	assert(1 <= a_writer -> num_bits_left && a_writer -> num_bits_left <= 8);
}

void flush_bit_writer(BitWriter* a_writer) {
	// unused bits should be filled with zeroes on the right side.
	if(a_writer -> num_bits_left != 8) {
		write_bits(a_writer, 0x00, a_writer -> num_bits_left);
		a_writer -> current_byte = 0;
		a_writer -> num_bits_left = 8;
	}
}

void close_bit_writer(BitWriter* a_writer) {
	flush_bit_writer(a_writer);
	fclose(a_writer -> file);
	a_writer -> file = NULL;
}

#define __BIT_WRITER_C_V1__
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab colorcolumn=96: */
