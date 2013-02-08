#ifndef STACK_H
#define  STACK_H

#include "types.h"

/* This stack frame is *not* run-time stack,  it's parse-time stack */

struct stack_frame {
	struct stack_frame *prev;
	struct pair *head;
	struct pair **tail;
	struct callable *cache;
	/* *
	 * when we parse list we may find first symbol's type in tree to
	 * determin eval_type, so we should cache it to avoid overhead of
	 * traverse the tree
	 */
};

extern void push_stack(struct stack_frame **f);
extern void pop_stack(struct stack_frame **f);
#endif /*STACK_H*/
