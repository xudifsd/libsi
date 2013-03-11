#pragma once

#include <stdio.h>

extern enum rtn_type u_read(struct pair *args, struct exp **rtn);
extern void interpret_file(FILE *in, FILE *out, FILE *err, struct environ *base_env);
extern void interpret_string(char *buf, FILE *out, FILE *err, struct environ *base_env);
