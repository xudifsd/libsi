#ifndef ENV_H
#define ENV_H

#include "tree.h"
#include "types.h"

extern int define_in_env(struct environ *env, struct symbol *sym, struct exp *value);
extern int set_in_env(struct environ *env, struct symbol *sym, struct exp *value);
extern struct exp *find_in_env(struct environ *env, struct symbol *sym);

#endif /* ENV_H */
