#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "stack.h"
#include "malloc.h"
#include "utils.h"
#include "eval.h"

extern int interpret(FILE *in, FILE *out, FILE *err, struct environ *base_env);
