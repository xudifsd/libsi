#include "utils.h"
#include "malloc.h"

void print(FILE *port, struct exp *e) {
	struct number *n;
	struct symbol *s;
	struct callable *c;
	struct pair *p;
	if (!e)
		fprintf(port, "()");
	else if (is_bool(e)) {
		if (((struct bool *)e)->value)
			fprintf(port, "#t");
		else
			fprintf(port, "#f");
	}
	else if (is_pair(e)) {
		fprintf(port, "(");
		for_pair(p, (struct pair *)e)
			print(port, (struct exp *)p->car);
		fprintf(port, "\b)");
	} else if (is_number(e)) {
		n = (struct number *)e;
		if (is_long(n))
			fprintf(port, "%ld", n->l_value);
		else
			fprintf(port, "%.8f", n->d_value);
	} else if (is_symbol(e)) {
		s = (struct symbol *)e;
		fprintf(port, "%s", s->sym);
	} else if (is_callable(e)) {
		c = (struct callable *)e;
		if (is_builtin_pro(c))
			fprintf(port, "#<builtin procedure>");
		else if (is_builtin_syntax(c))
			fprintf(port, "#<builtin syntax>");
		else if (is_lambda(c)) {
			fprintf(port, "#<lambda with pars ");
			print(port, (struct exp *)c->l_value.pars);
			fprintf(port, ">");
		} else {
			/* is_macro(c) */
			fprintf(port, "#<macro with pars ");
			print(port, (struct exp *)c->m_value.pars);
			fprintf(port, ">");
		}
	}
	fputc(' ', port); /* makes output more beautiful */
}

enum rtn_type map(map_f fun, struct pair *args, struct pair **rtn) {
	struct pair *p;
	struct pair *value;
	struct exp *e;
	struct pair *head = NULL;
	struct pair **tail = &head;
	enum rtn_type r_type;

	for_pair(p, args) {
		if ((r_type = fun(car(p), &e)) == SUCC) {
			value = alloc_pair(e, NULL);
			*tail = value;
			tail = (struct pair **)&value->cdr;
		} else
			return r_type;
	}
	*rtn = head;
	return SUCC;
}

/* if at_least is not 0, len should be at least nr_arg, not exactly */
enum rtn_type check_args(struct pair *args, unsigned int nr_arg, int at_least) {
	unsigned int len = 0;
	struct pair *p;

	for_pair(p, args)
		len++;

	if (at_least && len < nr_arg)
		return ERR_ARGC;
	else if (!at_least && len != nr_arg)
		return ERR_ARGC;
	else
		return SUCC;
}

struct exp *last_element(struct pair *head) {
	struct pair *p;
	for_pair(p, head)
		if (cdr(p) == NULL)
			return car(p);
	return NULL; /* avoid compile warning */
}

int above_zero(struct number *num) {
	if (is_long(num))
		return (num->l_value > 0);
	else
		return (num->d_value > 0);
}

int below_zero(struct number *num) {
	if (is_long(num))
		return (num->l_value < 0);
	else
		return (num->d_value < 0);
}

int equal_zero(struct number *num) {
	if (is_long(num))
		return (num->l_value == 0);
	else /* FIXME maybe we should not use == in double */
		return (num->d_value == 0);
}
