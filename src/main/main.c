#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <argp.h>
#include "huffman.h"

const char* argp_program_version = "huffmancli version 1.0";
const char* argp_program_bug_address = "<bug@example.com>";

static char doc[] = "This program compresses and uncompresses files with huffman coding.";

static char args_doc[] = "FILE";

static struct argp_option options[] = {
	//long_option short_option option_name has_arguments description
    {"uncompress", 'u', 0, 0, "Sets the program to uncompress a file instead of compressing", 0},
	{ 0 } // terminator
};

struct arguments {
	bool should_uncompress;
	char* args[1];
};

// Define what should happen when options get parsed
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
		case 'u':
			arguments->should_uncompress = true;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1) {
				argp_usage(state);
			}
			arguments->args[state->arg_num] = arg;
			break;
		default:
			return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char *argv[]) {
	// Command line arguments
    struct arguments arguments = { .should_uncompress = false }; // set default value for command line arguments
    argp_parse(&argp, argc, argv, 0, 0, &arguments); // parse command line arguments
	
	if(arguments.should_uncompress) {
		bool is_success = uncompress_file(arguments.args[0]);
		return is_success;
	}

	bool is_success = compress_file(arguments.args[0]);
	return is_success;
}

/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
