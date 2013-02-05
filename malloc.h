#ifndef MALLOC_H
#define  MALLOC_H

#include <gc.h>	/* bwd-gc */
#include <string.h>
#include "tree.h"
#include "types.h"

/* wrapper of bwd-gc, also init the object */

extern struct tree_node *alloc_tree_node(struct tree_node *parent, struct tree_node **pparent, struct symbol *sym, struct exp *e);
extern struct symbol *alloc_symbol(const char *str);
extern struct number *alloc_long(long value);
extern struct number *alloc_double(double value);
extern struct pair *alloc_pair(struct exp *car, struct exp *cdr);

#endif /*MALLOC_H*/
