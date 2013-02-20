#include "stack.h"
#include "malloc.h"
#include <assert.h>

void push_stack(struct stack_frame **f) {
	struct stack_frame *p = alloc_stack(*f);
	*f = p;
}

void pop_stack(struct stack_frame **f) {
	/* we do not free, because bwd-gc says it will be more efficient */
	*f = (*f)->prev;
}

void append_stack(struct stack_frame *f, struct pair *p) {
	*(f->tail) = p;
	f->tail = (struct pair **)&p->cdr;
}
