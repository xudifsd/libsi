#ifndef TREE_H
#define  TREE_H

#include <string.h>
#include "types.h"

enum tree_color {BLACK, RED};

struct tree_node {
	enum tree_color color;
	struct tree_node *parent;
	struct tree_node *left;
	struct tree_node *right;
	struct symbol *sym;
	struct exp *value;
};

struct tree_root {
	struct tree_node *node;
};

extern struct exp *tree_find(struct tree_root *root, const char *str);
extern void tree_insert(struct tree_root *root, struct symbol *sym, struct exp *e);

#endif /*TREE_H*/
