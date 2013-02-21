#ifndef STACK_H
#define  STACK_H

#include "types.h"

extern void push_stack(struct stack_frame **f);
extern void pop_stack(struct stack_frame **f);
extern void append_stack(struct stack_frame *f, struct exp *p);

extern void push_quote_stack(struct quote_stack **top);
extern void pop_quote_stack(struct quote_stack **top);
#endif /*STACK_H*/
