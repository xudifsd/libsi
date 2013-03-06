#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern void fatal(FILE *port, const char *fmt, ...);
extern void error(FILE *port, const char *fmt, ...);
