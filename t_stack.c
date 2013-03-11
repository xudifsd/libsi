#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "malloc.h"
#include "types.h"
#include "utils.h"

int main(int argc, char *argv[]) {
	/* *
	 * This is used to test stack construction, it's expected to print
	 * '(+ 5 (* 2 1) 10)'
	 */
	struct stack_frame *top = NULL;
	struct pair *p;
	struct symbol *s;
	struct number *n;
	int dot_list = 0;

	push_stack_frame(&top);

	s = alloc_symbol("+");
	append_stack(top, (struct exp *)s, &dot_list);

	n = alloc_long(5);
	append_stack(top, (struct exp *)n, &dot_list);

	push_stack_frame(&top);

	s = alloc_symbol("*");
	append_stack(top, (struct exp *)s, &dot_list);

	n = alloc_long(2);
	append_stack(top, (struct exp *)n, &dot_list);

	n = alloc_long(1);
	append_stack(top, (struct exp *)n, &dot_list);

	p = top->head;
	pop_stack_frame(&top);
	append_stack(top, (struct exp *)p, &dot_list);

	n = alloc_long(10);
	append_stack(top, (struct exp *)n, &dot_list);

	p = top->head;
	print(stdout, (struct exp *)p);
	printf("\n");
	exit(0);
}
