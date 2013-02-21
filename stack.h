#ifndef STACK_H
#define  STACK_H

#include "types.h"

/* This stack frame is *not* run-time stack,  it's parse-time stack */

struct stack_frame {
	struct stack_frame *prev;
	struct pair *head;
	struct pair **tail;
};

struct quote_stack {
	unsigned int nest;
	struct quote_stack *prev;
};


extern void push_stack(struct stack_frame **f);
extern void pop_stack(struct stack_frame **f);
extern void append_stack(struct stack_frame *f, struct exp *p);

extern void push_quote_stack(struct quote_stack **top);
extern void pop_quote_stack(struct quote_stack **top);
#endif /*STACK_H*/
