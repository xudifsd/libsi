#include "quote_stack.h"
#include "malloc.h"

void push_quote_stack(struct quote_stack **top) {
	*top = alloc_quote_stack(*top);
}

void pop_quote_stack(struct quote_stack **top) {
	/* we do not free, because bwd-gc says it will be more efficient */
	*top = (*top)->prev;
}
