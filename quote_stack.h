#ifndef QUOTE_STACK_H
#define  QUOTE_STACK_H

struct quote_stack {
	unsigned int nest;
	struct quote_stack *prev;
};

extern void push_quote_stack(struct quote_stack **top);
extern void pop_quote_stack(struct quote_stack **top);

#endif /*QUOTE_STACK_H*/
