#ifndef __BIT_READER_H__
#define __BIT_READER_H__

#include <stdio.h> // for FILE type
#include <stdint.h> // for uint8_t

typedef struct _BitReader {
	FILE* file;
	uint16_t current_byte;
	uint16_t num_bits_left;
	uint16_t num_bits_read;
} BitReader;

BitReader open_bit_reader(char const* path);
void read_bits(BitReader* a_reader, uint8_t num_bits_to_read);
uint8_t reader_tell(BitReader reader);
void get_byte_reader(BitReader* a_reader);
void close_bit_reader(BitReader* a_reader);

#endif
