#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <si/types.h>
#include <si/setup.h>
#include <si/interpret.h>

#include <readline/history.h>
#include <readline/readline.h>

char buf[4096];

int cpy_block(char *dest, char *src) {
	static int nest = 0; /* we use nest to determine if has list terminated */
	for (; *src != '\0'; src++, dest++) {
		if (*src == '(')
			nest++;
		else if (*src == ')')
			nest--;
		*dest = *src;
	}
	*dest = ' '; /* 1 is for extra whitspace */
	dest++;
	*dest = '\0';
	return nest;
}

int main(int argc, char *argv[]) {
	struct environ *base;
	char *read_buf;
	int offset = 0;
	base = setup_builtin();

	if (setup_basic(base)) {
		fprintf(stderr, "error in setup_builtin\n");
		exit(1);
	}
	for (;;) {
		read_buf = readline(offset? "---> ": "si> ");
		if (!read_buf)
			break;
		if (cpy_block(buf + offset, read_buf)) {
			offset += strlen(read_buf) + 1; /* 1 is for extra whitspace */
			continue;
		} else
			offset = 0;
		interpret_string(buf, stdout, stderr, base);
		free(read_buf);
	}
	return 0;
}
