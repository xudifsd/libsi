#ifndef ENV_H
#define ENV_H

#include "tree.h"
#include "types.h"
#include "malloc.h"
#include "utils.h"

extern int define_in_env(struct environ *env, struct symbol *sym, struct exp *value);
extern int set_in_env(struct environ *env, struct symbol *sym, struct exp *value);
extern struct exp *find_in_env(struct environ *env, struct symbol *sym);
extern struct environ *extend_env(struct pair *pars, struct pair *args, struct environ *base);

#endif /* ENV_H */
