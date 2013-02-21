#include "malloc.h"
#include <assert.h>

struct environ *alloc_environ(struct environ *parent) {
	struct environ *rtn = (struct environ *)GC_MALLOC(sizeof(struct environ));
	assert(rtn);
	rtn->parent = parent;
	rtn->repo = NULL;
	return rtn;
}

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

struct stack_frame *alloc_stack(struct stack_frame *prev) {
	struct stack_frame *rtn = (struct stack_frame *)GC_MALLOC(sizeof(struct stack_frame));
	assert(rtn);
	rtn->prev = prev;
	rtn->head = NULL;
	rtn->tail = &rtn->head;
	return rtn;
}

struct quote_stack *alloc_quote_stack(struct quote_stack *prev) {
	struct quote_stack *rtn = (struct quote_stack *)GC_MALLOC(sizeof(struct quote_stack));
	assert(rtn);
	rtn->prev = prev;
	rtn->nest = 0;
	return rtn;
}

struct callable *alloc_builtin_pro(builtin_pro_f fun) {
	struct callable *rtn = GC_MALLOC_ATOMIC(sizeof(struct callable));
	assert(rtn);
	rtn->tag = CALLABLE;
	rtn->type = BUILTIN_PRO;
	rtn->bp_value = fun;
	return rtn;
}
