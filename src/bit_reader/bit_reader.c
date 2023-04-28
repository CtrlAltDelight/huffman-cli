#include <stdio.h>
#include <stdint.h>
#include "bit_reader.h"

static BitReader open_bit_reader(FILE* path) {
	return (BitReader) { .file = fopen(path, "r"), .current_byte = 0, .num_bits_left = 8 };
}

static void read_bits(BitReader* a_reader, num_bits_to_read) {
	if(a_reader -> num_bits_left == 0) {
		a_reader -> current_byte = fgetc(a_reader -> file);
		a_reader -> num_bits_left = 8;
	}
	a_reader -> num_bits_left -= num_bits_to_read;
	a_reader -> current_bits = 0;
}

static void close_bit_reader(BitReader* a_reader) {
	fclose(a_reader -> file);
	a_reader -> file = NULL;
}

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

