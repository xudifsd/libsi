#include "malloc.h"
#include <assert.h>

struct tree_node *alloc_tree_node(struct tree_node *parent, struct tree_node **pparent,
		struct symbol *sym, struct exp *e) {
	struct tree_node *rtn = (struct tree_node *)GC_MALLOC(sizeof(struct tree_node));
	assert(rtn != NULL);
	rtn->color = RED;	/* makes insert easier */
	rtn->parent = parent;
	rtn->pparent = pparent;
	rtn->left = NULL;
	rtn->right = NULL;
	rtn->sym = sym;
	rtn->value = e;
	return rtn;
}

struct symbol *alloc_symbol(const char *str) {
	struct symbol *rtn = (struct symbol *)GC_MALLOC_ATOMIC(sizeof(struct symbol));
	rtn->tag = SYMBOL;
	strcpy((char *)rtn->sym, str);
	return rtn;
}

struct number *alloc_long(long value) {
	struct number *rtn = (struct number *)GC_MALLOC_ATOMIC(sizeof(struct number));
	rtn->tag = NUMBER;
	rtn->type = LONG;
	rtn->l_value = value;
	return rtn;
}

struct number *alloc_double(double value) {
	struct number *rtn = (struct number *)GC_MALLOC_ATOMIC(sizeof(struct number));
	rtn->tag = NUMBER;
	rtn->type = DOUBLE;
	rtn->d_value = value;
	return rtn;
}

struct pair *alloc_pair(struct exp *car, struct exp *cdr) {
	struct pair *rtn = (struct pair *)GC_MALLOC(sizeof(struct pair));
	rtn->tag = PAIR;
	rtn->car = car;
	rtn->cdr = cdr;
	return rtn;
}
