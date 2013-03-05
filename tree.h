#ifndef TREE_H
#define  TREE_H

#include <string.h>
#include "types.h"

enum tree_color {BLACK, RED};

struct tree_node {
	enum tree_color color;
	struct tree_node *parent;
	struct tree_node **pparent;
	struct tree_node *left;
	struct tree_node *right;
	struct symbol *sym;
	struct exp *value;
};

extern int tree_find(struct tree_node *root, const char *str, struct exp **rtn);
extern int tree_insert(struct tree_node **root, struct symbol *sym, struct exp *e, int replace);
extern void dump(struct tree_node *root); /* for debugging */

#endif /*TREE_H*/
