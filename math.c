#include "math.h"

/* helper function for add and mul, type 0 for add, 1 for mul */
static struct number *construct_number(long l, double d, int type) {
	struct number *rtn;
	if (!type) {
		if (d == 0)
			rtn = alloc_long(l);
		else {
			d += l;
			rtn = alloc_double(d);
		}
	} else {
		if (d == 1)
			rtn = alloc_long(l);
		else {
			d *= l;
			rtn = alloc_double(d);
		}
	}
	return rtn;
}

#define math_template(name, init, sym) \
enum rtn_type name(struct pair *args, struct exp **rtn) {\
	long l_sum = init;\
	double d_sum = init;\
	struct pair *p;\
	struct number *num;\
	if (!args)\
		return ERR_ARGC;\
\
	for_pair(p, args) {\
		if (!is_number(p->car))\
			return ERR_TYPE;\
		num = (struct number *)(p->car);\
\
		if (is_long(num)) {\
			l_sum sym##= num->l_value;\
		} else if (is_double(num)) {\
			d_sum sym##= num->d_value;\
		} else\
			return ERR_TYPE;\
	}\
	*rtn = (struct exp *)construct_number(l_sum, d_sum, init);\
	return SUCC;\
}

math_template(add, 0, +)
math_template(mul, 1, *)
