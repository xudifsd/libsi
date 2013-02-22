#include "usage.h"

void fatal(FILE *port, const char *fmt, ...) {
	if (port) {
		va_list args;
		va_start(args, fmt);
		vfprintf(port, fmt, args);
		va_end(args);
		fputc('\n', port);
	}
	exit(1);
}

void error(FILE *port, const char *fmt, ...) {
	if (port) {
		va_list args;
		va_start(args, fmt);
		vfprintf(port, fmt, args);
		va_end(args);
		fputc('\n', port);
	}
}
