#include "stack.h"
#include "malloc.h"

void push_stack_frame(struct stack_frame **f) {
	struct stack_frame *p = alloc_stack_frame(*f);
	*f = p;
}

void pop_stack_frame(struct stack_frame **f) {
	/* we do not free, because bwd-gc says it will be more efficient */
	*f = (*f)->prev;
}

int append_stack(struct stack_frame *f, struct exp *p, int *dot) {
	struct pair *new;
	if (*dot && f->dot_list)
		return 1; /* error */
	else if (*dot) {
		new = (struct pair *)p;
		f->dot_list = 1;
		*dot = 0;
	} else
		new = alloc_pair(p, NULL);
	*(f->tail) = new;
	f->tail = (struct pair **)&new->cdr;
	return 0;
}


void push_quote_stack(struct quote_stack **top) {
	*top = alloc_quote_stack(*top);
}

void push_dot_stack(struct dot_stack **top) {
	*top = alloc_dot_stack(*top);
}

void pop_dot_stack(struct dot_stack **top) {
	/* we do not free, because bwd-gc says it will be more efficient */
	*top = (*top)->prev;
}

void pop_quote_stack(struct quote_stack **top) {
	/* we do not free, because bwd-gc says it will be more efficient */
	*top = (*top)->prev;
}

void push_stack(struct stack **f) {
	struct stack *p = alloc_stack(*f);
	*f = p;
}

void pop_stack(struct stack **f) {
	/* we do not free, because bwd-gc says it will be more efficient */
	*f = (*f)->prev;
}
