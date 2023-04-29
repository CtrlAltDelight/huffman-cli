#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__
#include <stdlib.h>
#include "frequencies.h"
#include "priority_queue.h"
#include "bit_writer.h"
#include "bit_reader.h"

typedef struct _TreeNode {
	uchar  character;
	size_t frequency;
	struct _TreeNode* left;
	struct _TreeNode* right;
} TreeNode;

typedef struct _BitCode {
	uint64_t bits;
	size_t num_bits;
} BitCode;

Node* make_huffman_pq(Frequencies freqs);
TreeNode* make_huffman_tree(Node* head);
void destroy_huffman_tree(TreeNode** a_root);

void write_coding_table(TreeNode* root, BitWriter* a_writer);
void write_compressed(TreeNode* root, BitWriter* a_writer, uint8_t const* uncompressed_bytes, size_t num_uncompressed_bytes);
bool compress_file(char const* path);

TreeNode* recreate_huffman_tree(BitReader* a_reader);
bool uncompress_file(char const* path);
void write_uncompressed(TreeNode* root, BitReader* a_reader, BitWriter* a_writer, unsigned int num_uncompressed_bytes);

#endif
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
