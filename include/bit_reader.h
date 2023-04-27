#ifndef __BIT_READER_H__
#define __BIT_READER_H__

#include <stdio.h> // for FILE type
#include <stdint.h> // for uint8_t

typedef struct _BitReader {
	FILE* file;
	uint8_t current_byte;
	uint8_t num_bits_read;
} BitReader;

#endif
