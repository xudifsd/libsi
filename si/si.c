#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <si/types.h>
#include <si/setup.h>
#include <si/interpret.h>

#include <readline/history.h>
#include <readline/readline.h>

int main(int argc, char *argv[]) {
	struct environ *base;
	char *buf;
	base = setup_builtin();
	for (;;) {
		buf = readline("si> ");
		if (!buf)
			break;
		interpret_string(buf, stdout, stderr, base);
		free(buf);
	}
	return 0;
}
