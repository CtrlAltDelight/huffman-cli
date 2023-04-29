#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "huffman.h"

static int _cmp_huffman(void const* a_left_value, void const* a_right_value) {
	const TreeNode* left_node  = a_left_value;
	const TreeNode* right_node = a_right_value;

	/* // This is much simpler than I expected, though less robust.
	bool is_left_node_cluster = left_node->left  != NULL && left_node->right  != NULL;
	bool is_right_node_cluster = right_node->left != NULL && right_node->right != NULL;

	// Sort by frequency
	if(left_node -> frequency != right_node -> frequency) {
		return left_node -> frequency - right_node -> frequency;
	}
                                     // If frequencies are the same...
	if(is_left_node_cluster) {       // newer clusters to the right of older clusters
		return 1;
	}
	else if(is_right_node_cluster) { // leaf nodes to the left of clusters
		return -1;
	}
	else {                           // if both are leaves, sort by ASCII value
		return left_node -> character - right_node -> character;
	}
	*/
	return left_node -> frequency - right_node -> frequency;
}

TreeNode* make_huffman_tree(Node* head) {
	if(head == NULL) {
		return NULL;
	}

	while(head -> next != NULL) {
		// Convenience variables.
		TreeNode* left_node  = head -> a_value;
		TreeNode* right_node = head -> next -> a_value;

		// Make a new cluster node using the trees in the first two nodes
		TreeNode* new_cluster_node = malloc(sizeof(*new_cluster_node));
		*new_cluster_node = (TreeNode) {
            .character = '\0',
            .frequency = right_node -> frequency + left_node -> frequency,
            .left = left_node,
            .right = right_node
		};

		// Destroy the first two nodes
		free(pq_dequeue(&head));
		free(pq_dequeue(&head));

		pq_enqueue(&head, new_cluster_node, _cmp_huffman); // enqueue the new cluster
	}

	TreeNode* root = head -> a_value; // save the root to return
	free(head); // destroy the node holding the root.

	return root;
}

void destroy_huffman_tree(TreeNode** a_root) {
	if(*a_root != NULL) {                          // If tree is not empty...
		destroy_huffman_tree(&(*a_root) -> left);  // Traverse the left subtree.
		destroy_huffman_tree(&(*a_root) -> right); // Traverse the right subtree.
		free(*a_root);                             // Free (destroy) the root.
	}
	*a_root = NULL;                                // Set the root to NULL in the caller's stack
}

Node* make_huffman_pq(Frequencies freqs) {
	Node* head = NULL; // Start with an empty list

	// For each character with a frequency...
	for(unsigned int freqs_idx = 0; freqs_idx < 256; freqs_idx++) {
		if(freqs[freqs_idx] > 0) {
			TreeNode* new_tree = malloc(sizeof(*new_tree)); // Make a treenode
			*new_tree = (TreeNode) { .character = freqs_idx, .frequency = freqs[freqs_idx] };
			pq_enqueue(&head, new_tree, _cmp_huffman);      // Enqueue it
		}
	}

	return head;
}

void write_coding_table(TreeNode* root, BitWriter* a_writer) {
	if(root == NULL) {
		return;
	}

	write_coding_table(root -> left,  a_writer); // Traverse left subtree
	write_coding_table(root -> right, a_writer); // Traverse right subtree

	// visit root
	if(root -> left != NULL && root -> right != NULL) { // If non-leaf or leaf
		write_bits(a_writer, 0x00, 1); // write a 0
	}
	else {
		write_bits(a_writer, 0x01, 1); // write a 1
		write_bits(a_writer, root -> character, 8); // write character
	}
}


static void _create_encoding_table(TreeNode* root, BitCode* encoding_table, uint8_t bit_code, 
																					int num_bits) {
	if(root == NULL) {
		return;
	}

	// Add a 0 to the code and traverse left
	_create_encoding_table(root -> left,  encoding_table,  bit_code << 1,         num_bits + 1);

	// Add a 1 to the code and traverse right
	_create_encoding_table(root -> right, encoding_table, (bit_code << 1) | 0x01, num_bits + 1);

	// Visit root
	if(root -> left == NULL && root -> right == NULL) { // Visit root if it is a leaf
		encoding_table[root -> character] = (BitCode) { .bits = bit_code, .num_bits = num_bits };
	}
}

void write_compressed(TreeNode* root, BitWriter* a_writer, uint8_t const* uncompressed_bytes, size_t num_uncompressed_bytes) {
	BitCode codes[256] = { { .bits = 0, .num_bits = 0 } }; // Cache of codes for each ascii value
	_create_encoding_table(root, codes, 0x00, 0); // Create the encoding table
	for(int i = 0; i < num_uncompressed_bytes; i++) {
		write_bits(a_writer, codes[uncompressed_bytes[i]].bits, codes[uncompressed_bytes[i]].num_bits); // Write the compressed bits for each character in uncompressed_bytes
	}
}

static uint8_t const* _read_file(char const* path) {
	FILE* file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	size_t num_bytes = ftell(file);
	uint8_t* file_contents = malloc(sizeof(*file_contents) * num_bytes);
	fseek(file, 0, SEEK_SET);
	fread(file_contents, sizeof(*file_contents), num_bytes, file);
	fclose(file);
	return file_contents;
}

bool compress_file(char const* path) {
	Frequencies freqs = {0};
	const char* error;
	if(!calc_frequencies(freqs, path, &error)) {
		printf("calc_frequencies failed: %s\n", error);
		return false;
	}
	Node* head = make_huffman_pq(freqs);
	TreeNode* root = make_huffman_tree(head);

	// Add .huff to the path
	char* compressed_path = malloc(sizeof(*compressed_path) * (strlen(path) + 6)); // +6 for '\0' and .huff
	strncpy(compressed_path, path, strlen(path) + 1); // +1 to copy null terminator TODO: Check if safe.
	strcat(compressed_path, ".huff");

	BitWriter writer = open_bit_writer(compressed_path);
	// put a 4 byte unsigned integer as the 
	unsigned int num_bytes = root->frequency;
	write_bits(&writer, num_bytes >> (8 * 3), 8);
	write_bits(&writer, num_bytes >> (8 * 2), 8);
	write_bits(&writer, num_bytes >> (8 * 1), 8);
	write_bits(&writer, num_bytes, 8);

	write_coding_table(root, &writer);
	write_bits(&writer, 0x00, 1); // separate the coding table from the encoded bits with a 0
	uint8_t const* file_contents = _read_file(path);
	write_compressed(root, &writer, file_contents, num_bytes);
	close_bit_writer(&writer);
	return true;
}

TreeNode* recreate_huffman_tree(BitReader* a_reader) {
	Node* head = NULL;
	while(true) {
		read_bits(a_reader, 1);
		uint8_t bit = reader_tell(*a_reader);
		assert(bit == 0x01 || bit == 0x00);
		if(bit == 0x01) {
			read_bits(a_reader, 8);
			TreeNode* new_node = malloc(sizeof(*new_node));
			*new_node = (TreeNode) {
				.character = reader_tell(*a_reader)
			};

			stack_push(&head, new_node);
		}
		else {
			if(head != NULL && head -> next == NULL) {
				break;
			}
			TreeNode* new_node = malloc(sizeof(*new_node));
			*new_node = (TreeNode) {
				.right = head->a_value,
				.left = head->next->a_value,
				.character = '\0'
			};
			free(stack_pop(&head));
			free(stack_pop(&head));
			stack_push(&head, new_node);
		}
	}
	TreeNode* root = head -> a_value;
	free(stack_pop(&head));
	return root;
}

static void _write_character_from_tree_code(TreeNode* root, BitReader* a_reader, BitWriter* a_writer, unsigned int* a_num_uncompressed_bytes) {
	// If at a leaf, print the ASCII value
	if(*a_num_uncompressed_bytes <= 0) {
		return;
	}
	if(root -> left == NULL && root -> right == NULL) {
		write_bits(a_writer, root -> character, 8);
		*a_num_uncompressed_bytes -= 1;
		return;
	}

	// read the next bit
	read_bits(a_reader, 1);
	uint8_t bit = reader_tell(*a_reader);
	assert(bit == 0x00 || bit == 0x01);

	// traverse in the direction of the bit
	if(bit == 0x00) {
		_write_character_from_tree_code(root -> left, a_reader, a_writer, a_num_uncompressed_bytes);
	}
	else if(bit == 0x01) {
		_write_character_from_tree_code(root -> right, a_reader, a_writer, a_num_uncompressed_bytes);
	}
}

void write_uncompressed(TreeNode* root, BitReader* a_reader, BitWriter* a_writer, unsigned int num_uncompressed_bytes) {
	while(!feof(a_reader->file) && num_uncompressed_bytes > 0) {
		_write_character_from_tree_code(root, a_reader, a_writer, &num_uncompressed_bytes);
	}
}

bool uncompress_file(char const* path) {
	// Add .unhuff to the path
	char* uncompressed_path = malloc(sizeof(*uncompressed_path) * (strlen(path) + 8)); // +6 for '\0' and .unhuff
	strncpy(uncompressed_path, path, strlen(path) + 1); // +1 to copy null terminator TODO: Check if safe.
	strcat(uncompressed_path, ".unhuff");

	BitReader reader = open_bit_reader(path);
	// read the unsigned 4 byte int
	read_bits(&reader, 8);
	unsigned int num_uncompressed_bytes = reader_tell(reader);
	read_bits(&reader, 8);
	num_uncompressed_bytes = (num_uncompressed_bytes << 8) | reader_tell(reader);
	read_bits(&reader, 8);
	num_uncompressed_bytes = (num_uncompressed_bytes << 8) | reader_tell(reader);
	read_bits(&reader, 8);
	num_uncompressed_bytes = (num_uncompressed_bytes << 8) | reader_tell(reader);
	printf("num_uncompressed == %d\n", num_uncompressed_bytes);

	TreeNode* root = recreate_huffman_tree(&reader);

	BitWriter writer = open_bit_writer(uncompressed_path);
	write_uncompressed(root, &reader, &writer, num_uncompressed_bytes);
	close_bit_reader(&reader);
	close_bit_writer(&writer);

	return true;
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
