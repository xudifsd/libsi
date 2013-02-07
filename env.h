#ifndef ENV_H
#define ENV_H

#include "tree.h"
#include "types.h"

struct envrion {
	struct envrion *parent;
	struct tree_node *repo;
};

extern int define_in_env(struct envrion *env, struct symbol *sym, struct exp *value);
extern int set_in_env(struct envrion *env, struct symbol *sym, struct exp *value);
extern struct exp *find_in_env(struct envrion *env, const char *sym);

#endif /* ENV_H */
