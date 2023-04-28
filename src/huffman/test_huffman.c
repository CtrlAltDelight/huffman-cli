#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "huffman.h"
#include "miniunit.h"

static void _delete_value_fn(void* a_value) {
	TreeNode* tree_node = a_value;
	free(tree_node);
}

static int _test_freqs() {
	mu_start();
	//─────────────
	Frequencies freqs = {0};
	const char* error = "";
	calc_frequencies(freqs, "./test.txt", &error); // Gets frequencies

	Node* head = make_huffman_pq(freqs);
	mu_check(head != NULL);
	//log_char(((TreeNode*) head -> a_value) -> character);
	mu_check( ( (TreeNode*) head -> a_value ) -> character == '\n' );
	mu_check( ( (TreeNode*) head -> a_value ) -> frequency == 1 );
	mu_check( ( (TreeNode*) head -> next -> a_value) -> character == 'C');
	mu_check( ( (TreeNode*) head -> next -> a_value) -> frequency == 1);
	mu_check( ( (TreeNode*) head -> next -> next -> a_value) -> character == 'B');
	mu_check( ( (TreeNode*) head -> next -> next -> a_value) -> frequency == 2 );
	mu_check( ( (TreeNode*) head -> next -> next -> next -> a_value) -> character == 'A');
	mu_check( ( (TreeNode*) head -> next -> next -> next -> a_value) -> frequency == 3);

	destroy_list(&head, _delete_value_fn);

	//─────────────
	mu_end();
}

static int _test_huffman() {
	mu_start();
	//─────────────
	Frequencies freqs = {0};
	const char* error = "";
	calc_frequencies(freqs, "./test.txt", &error); // Gets frequencies

	Node* head = make_huffman_pq(freqs);
	mu_check(head != NULL);
	//log_char(((TreeNode*) head -> a_value) -> character);
	mu_check( ( (TreeNode*) head -> a_value ) -> character == '\n' );
	mu_check( ( (TreeNode*) head -> a_value ) -> frequency == 1 );
	mu_check( ( (TreeNode*) head -> next -> a_value) -> character == 'C');
	mu_check( ( (TreeNode*) head -> next -> a_value) -> frequency == 1);
	mu_check( ( (TreeNode*) head -> next -> next -> a_value) -> character == 'B');
	mu_check( ( (TreeNode*) head -> next -> next -> a_value) -> frequency == 2 );
	mu_check( ( (TreeNode*) head -> next -> next -> next -> a_value) -> character == 'A');
	mu_check( ( (TreeNode*) head -> next -> next -> next -> a_value) -> frequency == 3);

	TreeNode* tree_node = make_huffman_tree(head);
	mu_check(tree_node != NULL);
	mu_check(tree_node -> character == '\0');
	mu_check(tree_node -> frequency == 7);
	mu_check(tree_node -> left != NULL);
	mu_check(tree_node -> left -> character == 'A');
	mu_check(tree_node -> left -> frequency == 3);
	mu_check(tree_node -> left -> left == NULL);
	mu_check(tree_node -> left -> right == NULL);
	mu_check(tree_node -> right != NULL);
	mu_check(tree_node -> right -> character == '\0');
	mu_check(tree_node -> right -> frequency == 4);
	mu_check(tree_node -> right -> left != NULL);
	mu_check(tree_node -> right -> left -> character == 'B');
	mu_check(tree_node -> right -> left -> frequency == 2);
	mu_check(tree_node -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> left -> right == NULL);
	mu_check(tree_node -> right -> right != NULL);
	mu_check(tree_node -> right -> right -> character == '\0');
	mu_check(tree_node -> right -> right -> frequency == 2);
	mu_check(tree_node -> right -> right -> left != NULL);
	mu_check(tree_node -> right -> right -> left -> character == '\n');
	mu_check(tree_node -> right -> right -> left -> frequency == 1);
	mu_check(tree_node -> right -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> right -> left -> right == NULL);
	mu_check(tree_node -> right -> right -> right != NULL);
	mu_check(tree_node -> right -> right -> right -> character == 'C');
	mu_check(tree_node -> right -> right -> right -> frequency == 1);
	mu_check(tree_node -> right -> right -> right -> left == NULL);
	mu_check(tree_node -> right -> right -> right -> right == NULL);

	destroy_huffman_tree(&tree_node);

	Frequencies freqs2 = {0};
	error = "";
	calc_frequencies(freqs2, "./test2.txt", &error); // Gets frequencies

	head = make_huffman_pq(freqs2);
	mu_check(head != NULL);
	tree_node = make_huffman_tree(head);

	destroy_huffman_tree(&tree_node);

	Frequencies freqs3 = {0};
	error = "";
	calc_frequencies(freqs3, "./test3.txt", &error); // Gets frequencies
	freqs3[10] = 0;

	head = make_huffman_pq(freqs3);
	mu_check(head != NULL);
	tree_node = make_huffman_tree(head);

	mu_check(tree_node != NULL);
	mu_check(tree_node -> character == '\0');
	mu_check(tree_node -> frequency == 24);
	mu_check(tree_node -> left != NULL);
	mu_check(tree_node -> left -> character == '\0');
	mu_check(tree_node -> left -> frequency == 10);
	mu_check(tree_node -> left -> left != NULL);
	mu_check(tree_node -> left -> left -> character == 'f');
	mu_check(tree_node -> left -> left -> frequency == 5);
	mu_check(tree_node -> left -> left -> left == NULL);
	mu_check(tree_node -> left -> left -> right == NULL);
	mu_check(tree_node -> left -> right != NULL);
	mu_check(tree_node -> left -> right -> character == '\0');
	mu_check(tree_node -> left -> right -> frequency == 5);
	mu_check(tree_node -> left -> right -> left -> character == '\0');
	mu_check(tree_node -> left -> right -> left -> frequency == 2);
	mu_check(tree_node -> left -> right -> right -> character == ' ');
	mu_check(tree_node -> left -> right -> right -> frequency == 3);
	mu_check(tree_node -> left -> right -> left -> left -> character == 'h');
	mu_check(tree_node -> left -> right -> left -> left -> frequency == 1);
	mu_check(tree_node -> left -> right -> left -> left -> left == NULL);
	mu_check(tree_node -> left -> right -> left -> left -> right == NULL);
	mu_check(tree_node -> left -> right -> left -> right -> character == 'l');
	mu_check(tree_node -> left -> right -> left -> right -> frequency == 1);
	mu_check(tree_node -> left -> right -> left -> right -> left == NULL);
	mu_check(tree_node -> left -> right -> left -> right -> right == NULL);
	mu_check(tree_node -> right != NULL);
	mu_check(tree_node -> right -> character == '\0');
	mu_check(tree_node -> right -> frequency == 14);
	mu_check(tree_node -> right -> left != NULL);
	mu_check(tree_node -> right -> left -> character == '\0');
	mu_check(tree_node -> right -> left -> frequency == 6);
	mu_check(tree_node -> right -> left -> left -> character == 'u');
	mu_check(tree_node -> right -> left -> left -> frequency == 3);
	mu_check(tree_node -> right -> left -> left -> left == NULL);
	mu_check(tree_node -> right -> left -> left -> right == NULL);
	mu_check(tree_node -> right -> left -> right -> character == '\0');
	mu_check(tree_node -> right -> left -> right -> frequency == 3);
	mu_check(tree_node -> right -> left -> right -> left -> character == 'y');
	mu_check(tree_node -> right -> left -> right -> left -> frequency == 1);
	mu_check(tree_node -> right -> left -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> left -> right -> left -> right == NULL);
	mu_check(tree_node -> right -> left -> right -> right -> character == 'a');
	mu_check(tree_node -> right -> left -> right -> right -> frequency == 2);
	mu_check(tree_node -> right -> left -> right -> right -> left == NULL);
	mu_check(tree_node -> right -> left -> right -> right -> right == NULL);
	mu_check(tree_node -> right -> right -> character == '\0');
	mu_check(tree_node -> right -> right -> frequency == 8);
	mu_check(tree_node -> right -> right -> left -> character == 'm');
	mu_check(tree_node -> right -> right -> left -> frequency == 4);
	mu_check(tree_node -> right -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> right -> left -> right == NULL);
	mu_check(tree_node -> right -> right -> right -> character == '\0');
	mu_check(tree_node -> right -> right -> right -> frequency == 4);
	mu_check(tree_node -> right -> right -> right -> left -> character == 'n');
	mu_check(tree_node -> right -> right -> right -> left -> frequency == 2);
	mu_check(tree_node -> right -> right -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> right -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> right -> right -> right -> character == 's');
	mu_check(tree_node -> right -> right -> right -> right -> frequency == 2);
	mu_check(tree_node -> right -> right -> right -> right -> left == NULL);
	mu_check(tree_node -> right -> right -> right -> right -> right == NULL);

	destroy_huffman_tree(&tree_node);

	
	Frequencies freqs4 = {0};
	error = "";
	calc_frequencies(freqs4, "./test4.txt", &error); // Gets frequencies
	freqs3[10] = 0;

	head = make_huffman_pq(freqs4);
	mu_check(head != NULL);
	tree_node = make_huffman_tree(head);

	mu_check(tree_node != NULL);
	mu_check(tree_node -> character == '\n');
	mu_check(tree_node -> frequency == 1);
	mu_check(tree_node -> left == NULL);
	mu_check(tree_node -> right == NULL);

	destroy_huffman_tree(&tree_node);
	mu_check(tree_node == NULL);
	//─────────────
	mu_end();
}

static int _test_null_huffman() {
	mu_start();
	//─────────────
	mu_check(make_huffman_tree(NULL) == NULL);
	//─────────────
	mu_end();
}

static int _test_header() {
	mu_start();
	//─────────────
	Frequencies freqs = {0};
	const char* error = "";
	calc_frequencies(freqs, "./test3.txt", &error); // Gets frequencies
	freqs[10] = 0;
	Node* head = make_huffman_pq(freqs);

	mu_check(head != NULL);
	TreeNode* tree_node = make_huffman_tree(head);

	mu_check(tree_node != NULL);
	mu_check(tree_node -> character == '\0');
	mu_check(tree_node -> frequency == 24);
	mu_check(tree_node -> left != NULL);
	mu_check(tree_node -> left -> character == '\0');
	mu_check(tree_node -> left -> frequency == 10);
	mu_check(tree_node -> left -> left != NULL);
	mu_check(tree_node -> left -> left -> character == 'f');
	mu_check(tree_node -> left -> left -> frequency == 5);
	mu_check(tree_node -> left -> left -> left == NULL);
	mu_check(tree_node -> left -> left -> right == NULL);
	mu_check(tree_node -> left -> right != NULL);
	mu_check(tree_node -> left -> right -> character == '\0');
	mu_check(tree_node -> left -> right -> frequency == 5);
	mu_check(tree_node -> left -> right -> left -> character == '\0');
	mu_check(tree_node -> left -> right -> left -> frequency == 2);
	mu_check(tree_node -> left -> right -> right -> character == ' ');
	mu_check(tree_node -> left -> right -> right -> frequency == 3);
	mu_check(tree_node -> left -> right -> left -> left -> character == 'h');
	mu_check(tree_node -> left -> right -> left -> left -> frequency == 1);
	mu_check(tree_node -> left -> right -> left -> left -> left == NULL);
	mu_check(tree_node -> left -> right -> left -> left -> right == NULL);
	mu_check(tree_node -> left -> right -> left -> right -> character == 'l');
	mu_check(tree_node -> left -> right -> left -> right -> frequency == 1);
	mu_check(tree_node -> left -> right -> left -> right -> left == NULL);
	mu_check(tree_node -> left -> right -> left -> right -> right == NULL);
	mu_check(tree_node -> right != NULL);
	mu_check(tree_node -> right -> character == '\0');
	mu_check(tree_node -> right -> frequency == 14);
	mu_check(tree_node -> right -> left != NULL);
	mu_check(tree_node -> right -> left -> character == '\0');
	mu_check(tree_node -> right -> left -> frequency == 6);
	mu_check(tree_node -> right -> left -> left -> character == 'u');
	mu_check(tree_node -> right -> left -> left -> frequency == 3);
	mu_check(tree_node -> right -> left -> left -> left == NULL);
	mu_check(tree_node -> right -> left -> left -> right == NULL);
	mu_check(tree_node -> right -> left -> right -> character == '\0');
	mu_check(tree_node -> right -> left -> right -> frequency == 3);
	mu_check(tree_node -> right -> left -> right -> left -> character == 'y');
	mu_check(tree_node -> right -> left -> right -> left -> frequency == 1);
	mu_check(tree_node -> right -> left -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> left -> right -> left -> right == NULL);
	mu_check(tree_node -> right -> left -> right -> right -> character == 'a');
	mu_check(tree_node -> right -> left -> right -> right -> frequency == 2);
	mu_check(tree_node -> right -> left -> right -> right -> left == NULL);
	mu_check(tree_node -> right -> left -> right -> right -> right == NULL);
	mu_check(tree_node -> right -> right -> character == '\0');
	mu_check(tree_node -> right -> right -> frequency == 8);
	mu_check(tree_node -> right -> right -> left -> character == 'm');
	mu_check(tree_node -> right -> right -> left -> frequency == 4);
	mu_check(tree_node -> right -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> right -> left -> right == NULL);
	mu_check(tree_node -> right -> right -> right -> character == '\0');
	mu_check(tree_node -> right -> right -> right -> frequency == 4);
	mu_check(tree_node -> right -> right -> right -> left -> character == 'n');
	mu_check(tree_node -> right -> right -> right -> left -> frequency == 2);
	mu_check(tree_node -> right -> right -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> right -> right -> left -> left == NULL);
	mu_check(tree_node -> right -> right -> right -> right -> character == 's');
	mu_check(tree_node -> right -> right -> right -> right -> frequency == 2);
	mu_check(tree_node -> right -> right -> right -> right -> left == NULL);
	mu_check(tree_node -> right -> right -> right -> right -> right == NULL);

	BitWriter writer = open_bit_writer("./output.txt");
	write_coding_table(tree_node, &writer);
	flush_bit_writer(&writer);

	uint8_t input_string[5];
	input_string[0] = 'h';
	input_string[1] = 'u';
	input_string[2] = 'f';
	input_string[3] = 'f';
	input_string[4] = '\0';

	write_compressed(tree_node, &writer, input_string, 4);

	close_bit_writer(&writer);

	destroy_huffman_tree(&tree_node);


	//─────────────
	mu_end();
}

static bool _test_file_contents(const char* path, uint8_t* bytes, size_t num_bytes) {
	FILE* fp = fopen(path, "r");
	for(size_t i = 0; i < num_bytes; i++) {
		if(fgetc(fp) != bytes[i]) {
			fclose(fp);
			return false;
		}
	}
	fclose(fp);
	return true;
}

static uint8_t* _read_file(const char* path) {
	// Open the file at path
	FILE* fp = fopen(path, "r");

	// Find the size of the file in bytes
	fseek(fp, 0, SEEK_END);
	size_t num_bytes = ftell(fp);
	
	// Allocate and init string
	uint8_t* string = malloc(sizeof(*string) * (num_bytes + 1));
	fseek(fp, 0, SEEK_SET);
	for(size_t i = 0; i < num_bytes; i++) {
		string[i] = fgetc(fp);
	}
	string[num_bytes] = '\0';
	fclose(fp);

	return string;
}

/*
int _test_full_compression() {
	mu_start();

	// Calculate frequencies
	Frequencies freqs = {0};
	const char* error = "";
	bool is_successful = calc_frequencies(freqs, "test_uncompressed.txt", &error);
	mu_check(is_successful);
	mu_check(freqs['f'] != 0);

	// Make huffman pq
	Node* head = make_huffman_pq(freqs);
	mu_check(head != NULL);

	// Make the tree
	TreeNode* root = make_huffman_tree(head);
	mu_check(root != NULL);

	// Write header information
	BitWriter writer = open_bit_writer("test_compressed.txt");
	write_coding_table(root, &writer);
	write_bits(&writer, 0x00, 1); // 1 zero bit separates header information from compressed data

	uint8_t* string = _read_file("test_uncompressed.txt");
	write_compressed(root, &writer, string, );
	free(string);
	close_bit_writer(&writer);

	// Write compressed bytes

	destroy_huffman_tree(&root);
	uint8_t bytes[] = { 0xb3, 0x5a, 0x2d, 0x89, 0x01, 0x75, 0xbc, 0xd8, 0x4b, 0x6d, 0xba, 0xe6, 0x04, 0x81, 0xaf, 0x98, 0xb0, 0x3d, 0xeb, 0xea, 0x7a, 0x6f };
	mu_check(_test_file_contents("test_compressed.txt", bytes, 22));
			// 1011 0011 0101 1010 0010 1101 1000 1001 0000 0001 0111 0101 1011 1100 1101 1000 0100 1011 0110 1101 1011 1010 1110 0110 0000 0100 1000 0001 1010 1111 1001 1000 1011 0000 0011 1101 1110 1011 1110 1010 0111 1010 0110 1111
			// b3 5a 2d 89 01 75 bc d8 4b 6d ba e6 04 81 af 98 b0 3d eb ea 7a 6f

	mu_end();
}
*/

int _test_compress_file() {
	mu_start();
	mu_check(compress_file("huffman.txt"));
	mu_end();
}

int _test_read_header() {
	mu_start();

	char const* path = "huffman.txt";
	Frequencies freqs = {0};
	char const* error;
	calc_frequencies(freqs, path, &error);
	Node* head = make_huffman_pq(freqs);
	TreeNode* root = make_huffman_tree(head);
	char const* header_path = "header.txt";
	BitWriter writer = open_bit_writer(header_path);
	write_coding_table(root, &writer);
	close_bit_writer(&writer);

	TreeNode* re_root = recreate_huffman_tree(header_path);
	mu_check(re_root != NULL);
	mu_check(re_root -> left -> left -> character == 'f');
	mu_check(re_root -> left -> right -> left -> left -> character == 'h');
	mu_check(re_root -> left -> right -> left -> right -> character == 'l');
	mu_check(re_root -> left -> right -> right -> character == ' ');
	mu_check(re_root -> right -> left -> left -> character == 'u');
	mu_check(re_root -> right -> left -> right -> left -> character == 'y');
	mu_check(re_root -> right -> left -> right -> right -> character == 'a');
	mu_check(re_root -> right -> right -> left -> character == 'm');
	mu_check(re_root -> right -> right -> right -> left -> character == 'n');
	mu_check(re_root -> right -> right -> right -> right -> character == 's');

	mu_end();
}

int main(int argc, char* argv[]) {
	/*
	mu_run(_test_freqs);
	mu_run(_test_huffman);
	mu_run(_test_null_huffman);
	mu_run(_test_header);
	mu_run(_test_full_compression);
	*/
	//mu_run(_test_compress_file);
	mu_run(_test_read_header);
	return EXIT_SUCCESS;
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
