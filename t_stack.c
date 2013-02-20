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

	push_stack(&top);

	s = alloc_symbol("+");
	p = alloc_pair((struct exp *)s, NULL);
	append_stack(top, p);

	n = alloc_long(5);
	p = alloc_pair((struct exp *)n, NULL);
	append_stack(top, p);

	push_stack(&top);

	s = alloc_symbol("*");
	p = alloc_pair((struct exp *)s, NULL);
	append_stack(top, p);

	n = alloc_long(2);
	p = alloc_pair((struct exp *)n, NULL);
	append_stack(top, p);

	n = alloc_long(1);
	p = alloc_pair((struct exp *)n, NULL);
	append_stack(top, p);

	p = top->head;
	pop_stack(&top);
	p = alloc_pair((struct exp *)p, NULL);
	append_stack(top, p);

	n = alloc_long(10);
	p = alloc_pair((struct exp *)n, NULL);
	append_stack(top, p);

	p = top->head;
	print(stdout, (struct exp *)p);
	printf("\n");
	exit(0);
}
