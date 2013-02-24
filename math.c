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

#define template_for_log_sin_cos_tan(name)\
enum rtn_type u_##name(struct pair *args, struct exp **rtn) {\
	struct number *num;\
	double result;\
	struct exp *ar;\
	enum rtn_type r_type;\
\
	if ((r_type = check_args(args, 1)) != SUCC)\
		return r_type;\
\
	ar = car(args);\
	if (is_number(ar)) {\
		num = (struct number *)ar;\
		if (is_long(num))\
			result = name(num->l_value);\
		else\
			result = name(num->d_value);\
		if (isnormal(result)) {\
			*rtn = (struct exp *)alloc_double(result);\
			return SUCC;\
		} else\
			return ERR_MATH;\
	} else\
		return ERR_TYPE;\
}

template_for_log_sin_cos_tan(log);
template_for_log_sin_cos_tan(sin);
template_for_log_sin_cos_tan(cos);
template_for_log_sin_cos_tan(tan);
