#include "utils.h"
#include "malloc.h"

void print(FILE *port, struct exp *e) {
	struct number *n;
	struct symbol *s;
	struct pair *p;
	if (!e)
		fprintf(port, "() ");
	else if (is_pair(e)) {
		fprintf(port, "(");
		for_pair(p, (struct pair *)e)
			print(port, (struct exp *)p->car);
		fprintf(port, "\b) ");
	} else if (is_number(e)) {
		n = (struct number *)e;
		if (is_long(n)) {
			fprintf(port, "%ld ", n->l_value);
		} else {
			fprintf(port, "%.3f ", n->d_value);
		}
	} else if (is_symbol(e)) {
		s = (struct symbol *)e;
		fprintf(port, "%s ", s->sym);
	}
}

struct pair *map(map_f fun, struct pair *p) {
	struct pair *tmp;
	struct pair *rtn;
	struct pair *head;
	struct pair **tail = &head;
	for_pair(tmp, p) {
		rtn = alloc_pair(fun(car(tmp)), NULL);
		*tail = rtn;
		tail = (struct pair **)&rtn->cdr;
	}
	return head;
}
