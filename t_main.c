#include <stdio.h>
#include <string.h>
#include "setup.h"

/* *
 * this is the most basic scheme repl, compile it and run ./t_main
 * and then input something like '(+ 3 12 (* 32 2))', it will output 79
 */
int main(int argc, char *argv[]) {
	struct environ *base_env;
	FILE *in;
	int cont = 0;
	int file_index;

	if (argc == 2) {
		in = fopen(argv[1], "r");
		if (!in)
			fatal(stderr, "Couldn't open %s\n", argv[1]);
	} else if (argc == 3) {
		if (!strcmp(argv[1], "-r")) {
			file_index = 2;
			cont = 1;
		} else if (!strcmp(argv[2], "-r")) {
			file_index = 1;
			cont = 1;
		} else
			fatal(stderr, "Usage: %s -r file\n", argv[0]);
		in = fopen(argv[file_index], "r");
		if (!in)
			fatal(stderr, "Couldn't open %s\n", argv[file_index]);
	} else
		in = stdin;

	base_env = setup_builtin();

	interpret_file(in, stdout, stderr, base_env);
	if (cont)
		interpret_file(stdin, stdout, stderr, base_env);

	exit(0);
}
