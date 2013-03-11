#pragma once

#include "types.h"

extern void push_stack_frame(struct stack_frame **f);
extern void pop_stack_frame(struct stack_frame **f);
extern int append_stack(struct stack_frame *f, struct exp *p, int *dot);

extern void push_quote_stack(struct quote_stack **top);
extern void pop_quote_stack(struct quote_stack **top);

extern void push_stack(struct stack **f);
extern void pop_stack(struct stack **f);

extern void push_dot_stack(struct dot_stack **top);
extern void pop_dot_stack(struct dot_stack **top);
