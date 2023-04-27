#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"

static int _cmp_huffman(void const* a_left_value, void const* a_right_value) {
	const TreeNode* left_node  = a_left_value;
	const TreeNode* right_node = a_right_value;

	/*
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
		write_bits(a_writer, 0xff, 1); // write a 1
		write_bits(a_writer, root -> character, 8); // write character
	}
}


typedef struct _BitCode { // From hint, tried to make it static, but errors were thrown
	uint8_t bits;
	int     num_bits;
} BitCode;

static void _create_encoding_table(TreeNode* root, BitCode* encoding_table, uint8_t bit_code, 
																					int num_bits) {
	if(root == NULL) {
		return;
	}

	// Add a 0 to the code and traverse left
	_create_encoding_table(root -> left,  encoding_table,  bit_code << 1,         num_bits + 1);
	// add a 1 to the code and traverse right
	_create_encoding_table(root -> right, encoding_table, (bit_code << 1) | 0x01, num_bits + 1);
	if(root -> left == NULL && root -> right == NULL) { // Visit root if it is a leaf
		encoding_table[root -> character] = (BitCode) { .bits = bit_code, .num_bits = num_bits };
	}
}

void write_compressed(TreeNode* root, BitWriter* a_writer, uint8_t* uncompressed_bytes) {
	BitCode codes[256] = { { .bits = 0, .num_bits = 0 } }; // Initialize all codes to 0.
	_create_encoding_table(root, codes, 0x00, 0); // Create the encoding table
	for(int i = 0; uncompressed_bytes[i] != '\0'; i++) {
		write_bits(a_writer, codes[uncompressed_bytes[i]].bits, codes[uncompressed_bytes[i]].num_bits); // Write the compressed bits for each character in uncompressed_bytes
	}
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
