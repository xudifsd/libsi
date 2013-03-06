#pragma once

#include <string.h>
#include "tree.h"
#include "types.h"
#include "malloc.h"
#include "utils.h"

extern int define_in_env(struct environ *env, struct symbol *sym, struct exp *value);
extern int set_in_env(struct environ *env, struct symbol *sym, struct exp *value);
extern int find_in_env(struct environ *env, struct symbol *sym, struct exp **rtn);
extern struct environ *extend_env(struct pair *pars, struct pair *args, struct environ *base);
