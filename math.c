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

/* type 0 for add, 1 for mul */
static enum rtn_type wrapper_for_add_mul(struct pair *args, struct exp **rtn, int type) {
	long l_sum;
	double d_sum;
	struct pair *p;
	struct number *num;
	enum rtn_type r_type;

	l_sum = d_sum = type? 1: 0;

	if ((r_type = check_args(args, 1, 1, rtn)) != SUCC)
		return r_type;

	for_pair(p, args) {
		if (!is_pair(cdr(p)) && cdr(p) != NULL) {
			*rtn = (struct exp *)alloc_err_msg("args list is not a list");
			return ERR_TYPE;
		}
		if (!is_number(p->car)) {
			*rtn = (struct exp *)alloc_err_msg("args is not number");
			return ERR_TYPE;
		}
		num = (struct number *)(p->car);

		if (is_long(num)) {
			if (type)
				l_sum *= num->l_value;
			else
				l_sum += num->l_value;
		} else if (is_double(num)) {
			if (type)
				d_sum *= num->d_value;
			else
				d_sum += num->d_value;
		} else {
			*rtn = (struct exp *)alloc_err_msg("internal bug, unknow number type");
			return ERR_TYPE;
		}
	}
	*rtn = (struct exp *)construct_number(l_sum, d_sum, type);
	return SUCC;
}

enum rtn_type add(struct pair *args, struct exp **rtn) {
	return wrapper_for_add_mul(args, rtn, 0);
}

enum rtn_type mul(struct pair *args, struct exp **rtn) {
	return wrapper_for_add_mul(args, rtn, 1);
}

typedef double(*math_f)(double);

static enum rtn_type wrapper_for_math_function(struct pair *args, struct exp **rtn, math_f fun) {
	struct number *num;
	double result;
	struct exp *ar;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
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
		} else {
			*rtn = (struct exp *)alloc_err_msg("underlay library return unnormal result");
			return ERR_MATH;
		}
	} else {
		*rtn = (struct exp *)alloc_err_msg("args is not number");
		return ERR_TYPE;
	}
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

	if ((r_type = check_args(args, 1, 1, rtn)) != SUCC)
		return r_type;

	for_pair(p, args) {
		if (!is_pair(cdr(p)) && cdr(p) != NULL) {
			*rtn = (struct exp *)alloc_err_msg("args list is not a list");
			return ERR_TYPE;
		}
		if (!is_number(car(p))) {
			*rtn = (struct exp *)alloc_err_msg("args is not number");
			return ERR_TYPE;
		}
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
		} else {
			*rtn = (struct exp *)alloc_err_msg("internal bug, unknow number type");
			return ERR_TYPE;
		}
	}
	*rtn = (struct exp *)construct_number(l, d, 0);
	return SUCC;
}

enum rtn_type u_sqrt(struct pair *args, struct exp **rtn) {
	struct number *num;
	double in;
	struct exp *result;
	enum rtn_type r_type;

	if ((r_type = negative_p(args, &result)) != SUCC)
		return r_type;
	/* we do not support sqrt(-1) */
	if (is_bool(result) && is_true((struct bool *)result)) {
		*rtn = (struct exp *)alloc_err_msg("could not sqrt negative number");
		return ERR_MATH;
	}

	num = (struct number *)car(args);
	if (is_long(num))
		in = num->l_value;
	else if (is_double(num))
		in = num->d_value;
	else {
		*rtn = (struct exp *)alloc_err_msg("internal bug, unknow number type");
		return ERR_TYPE;
	}
	*rtn = (struct exp *)alloc_double(sqrt(in));
	return SUCC;
}
