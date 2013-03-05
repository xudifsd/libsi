#include "math.h"

/* FIXME we should have code cope with overflow */

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

/* FIXME use arguments to distinguish type, insteadt of macro */
#define template_for_add_mul(name, init, sym) \
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

template_for_add_mul(add, 0, +)
template_for_add_mul(mul, 1, *)

typedef double(*math_f)(double);

static enum rtn_type wrapper_for_math_function(struct pair *args, struct exp **rtn, math_f fun) {
	struct number *num;
	double result;
	struct exp *ar;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0)) != SUCC)
		return r_type;

	ar = car(args);
	if (is_number(ar)) {
		num = (struct number *)ar;
		if (is_long(num))
			result = fun(num->l_value);
		else
			result = fun(num->d_value);
		if (isnormal(result)) {
			*rtn = (struct exp *)alloc_double(result);
			return SUCC;
		} else
			return ERR_MATH;
	} else
		return ERR_TYPE;
}

enum rtn_type u_log(struct pair *args, struct exp **rtn) {
	return wrapper_for_math_function(args, rtn, log);
}

enum rtn_type u_sin(struct pair *args, struct exp **rtn) {
	return wrapper_for_math_function(args, rtn, sin);
}

enum rtn_type u_cos(struct pair *args, struct exp **rtn) {
	return wrapper_for_math_function(args, rtn, cos);
}

enum rtn_type u_tan(struct pair *args, struct exp **rtn) {
	return wrapper_for_math_function(args, rtn, tan);
}

enum rtn_type sub(struct pair *args, struct exp **rtn) {
	double d = 0;
	long l = 0;
	int first = 1;
	struct pair *p;
	struct number *num;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 1)) != SUCC)
		return r_type;

	for_pair(p, args) {
		if (!is_number(car(p)))
			return ERR_TYPE;
		num = (struct number *)car(p);
		if (is_long(num)) {
			if (first) {
				l = num->l_value;
				first = 0;
			} else
				l -= num->l_value;
		} else if (is_double(num)) {
			if (first) {
				d = num->d_value;
				first = 0;
			} else
				d -= num->d_value;
		} else
			return ERR_TYPE;
	}
	*rtn = (struct exp *)construct_number(l, d, 0);
	return SUCC;
}
