#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <si/types.h>
#include <si/setup.h>
#include <si/interpret.h>

#define BUF_SIZ 4096

char buf[BUF_SIZ];

/* to check if buf has full list */
int chk_block(char *src, int *offset) {
	/* we use nest to determine if list has terminated */
	static int nest = 0;
	src += *offset;
	for (; *src != '\0'; src++, (*offset)++) {
		if (*src == '(')
			nest++;
		else if (*src == ')')
			nest--;
	}
	return nest;
}

int main(int argc, char *argv[]) {
	struct environ *base;
	int offset = 0;
	FILE *in;

	if (argc == 2) {
		in = fopen(argv[1], "r");
		if (!in) {
			fprintf(stderr, "Couldn't open %s\n", argv[1]);
			exit(1);
		}
	} else
		in = stdin;

	base = setup_builtin();

	if (setup_basic(base)) {
		fprintf(stderr, "error in setup_builtin\n");
		exit(1);
	}
	if (in == stdin) {
		for (;;) {
			printf("%s ", offset? "--->": "si>");
			if (!fgets(buf + offset, BUF_SIZ - 1, stdin))
				break;
			if (chk_block(buf, &offset))
				continue;
			else
				offset = 0;
			interpret_string(buf, stdout, stderr, base);
		}
	} else
		interpret_file(in, stdout, stderr, base);
	return 0;
}
