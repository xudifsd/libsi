#ifndef MALLOC_H
#define  MALLOC_H

#include <gc.h>	/* bwd-gc */
#include <string.h>
#include "tree.h"
#include "env.h"
#include "types.h"
#include "stack.h"

/* wrapper of bwd-gc, also init the object */

extern struct bool *alloc_bool(int value);
extern struct environ *alloc_environ(struct environ *parent);
extern struct tree_node *alloc_tree_node(struct tree_node *parent, struct tree_node **pparent, struct symbol *sym, struct exp *e);
extern struct symbol *alloc_symbol(const char *str);
extern struct number *alloc_long(long value);
extern struct number *alloc_double(double value);
extern struct pair *alloc_pair(struct exp *car, struct exp *cdr);
extern struct stack_frame *alloc_stack(struct stack_frame *prev);
extern struct quote_stack *alloc_quote_stack(struct quote_stack *prev);
extern struct callable *alloc_builtin_pro(builtin_pro_f fun);
extern struct callable *alloc_builtin_syntax(builtin_syntax_f fun);
extern struct callable *alloc_lambda(struct pair *args, struct pair *body, struct environ *bind);
extern struct callable *alloc_macro(struct pair *pars, struct pair *body, struct environ *bind);

#endif /*MALLOC_H*/
