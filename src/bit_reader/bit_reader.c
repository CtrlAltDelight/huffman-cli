#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "bit_reader.h"

BitReader open_bit_reader(char const* path) {
	BitReader reader = { .file = fopen(path, "r"), .current_byte = 0x0000, .num_bits_left = 8, .num_bits_read = 0 }; // IDEA: store the most recent num_bits_to_read, so reader_tell(...) can display the latest requested bits.
	reader.current_byte = fgetc(reader.file);
	return reader;
}

void read_bits(BitReader* a_reader, uint8_t num_bits_to_read) {
	assert(0 <= num_bits_to_read && num_bits_to_read <= 8);
	assert(1 <= a_reader->num_bits_left && a_reader->num_bits_left <= 8);

	a_reader -> num_bits_read = num_bits_to_read;

	if(a_reader -> num_bits_left >= num_bits_to_read) {
		a_reader -> num_bits_left -= num_bits_to_read;
		if(a_reader -> num_bits_left <= 0) {
			get_byte_reader(a_reader);
			a_reader -> num_bits_left = 8;
		}
	}
	else {
		get_byte_reader(a_reader);
		num_bits_to_read -= a_reader -> num_bits_left;
		a_reader -> num_bits_left = 8 - num_bits_to_read;
	}

	assert(1 <= a_reader->num_bits_left && a_reader->num_bits_left <= 8);
	assert(1 <= a_reader->num_bits_read && a_reader -> num_bits_read <= 8);
}

uint8_t reader_tell(BitReader reader) { // tells the current_byte 
	uint8_t bit_mask = 0xff >> (8 - reader.num_bits_read);
	uint8_t right_aligned = reader.current_byte >> reader.num_bits_left;
	return right_aligned & bit_mask;
}

void get_byte_reader(BitReader* a_reader) {
	a_reader -> current_byte <<= 8;

	uint8_t new_byte = fgetc(a_reader -> file);
	if(!feof(a_reader -> file)) {
		a_reader -> current_byte |= new_byte;
	}
}

void close_bit_reader(BitReader* a_reader) {
	fclose(a_reader -> file);
	a_reader -> file = NULL;
}

/*
TreeNode* read_coding_table() {
	if(current_bit == 1) {
		TreeNode* new_leaf = malloc(sizeof(*new_node));
		*new_leaf = (TreeNode) { .character = read_byte(), .left = NULL, .right = NULL};
		return new_leaf;
	}
	else {
		TreeNode* left_node = read_coding_table();
		TreeNode* right_node = read_coding_table();
		TreeNode* new_cluster = malloc(sizeof(new_cluster));
		*new_cluster = { .character = '\0', .left = left_node, .right = right_node };
		return new_cluster;
	}
}
*/
