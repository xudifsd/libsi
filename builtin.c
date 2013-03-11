#include "builtin.h"

enum rtn_type length(struct pair *args, struct exp **rtn) {
	unsigned int len = 0;
	struct pair *p;
	enum rtn_type r_type;
	struct exp *r_args;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	r_args = car(args);

	if (!is_pair(r_args)) {
		*rtn = (struct exp *)alloc_err_msg("arg is not a list");
		return ERR_TYPE;
	}
	for_pair(p, (struct pair *)r_args) {
		if (!is_pair(cdr(p)) && cdr(p) != NULL) {
			*rtn = (struct exp *)alloc_err_msg("arg is not a list");
			return ERR_TYPE;
		}
		len++;
	}
	*rtn = (struct exp *)alloc_long(len);
	return SUCC;
}

enum rtn_type list(struct pair *args, struct exp **rtn) {
	*rtn = (struct exp *)args;
	return SUCC;
}

enum rtn_type cons(struct pair *args, struct exp **rtn) {
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
		return r_type;

	*rtn = (struct exp *)alloc_pair(car(args), car((struct pair *)cdr(args)));
	return SUCC;
}

/* type 0 for car, 1 for cdr */
static enum rtn_type wrapper_for_car_cdr(struct pair *args, struct exp **rtn, int type) {
	struct exp *r_args;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;
	r_args = car(args);
	if (is_pair(r_args)) {
		if (type == 0)
			*rtn = car((struct pair *)r_args);
		else
			*rtn = cdr((struct pair *)r_args);
		return SUCC;
	} else {
		*rtn = (struct exp *)alloc_err_msg("arg is not a pair");
		return ERR_TYPE;
	}
}
enum rtn_type u_car(struct pair *args, struct exp **rtn) {
	return wrapper_for_car_cdr(args, rtn, 0);
}

enum rtn_type u_cdr(struct pair *args, struct exp **rtn) {
	return wrapper_for_car_cdr(args, rtn, 1);
}

/* when succ, rtn contains a pair whose car is min, cdr is max */
static enum rtn_type min_max(struct pair *args, struct pair **rtn) {
	double cur_dmax = DBL_MIN;
	double cur_dmin = DBL_MAX;
	long cur_lmax = LONG_MIN;
	long cur_lmin = LONG_MAX;
	struct pair *p;
	struct number *n_min;
	struct number *n_max;
	struct number *num;
	struct exp *e;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 1, (struct exp **)rtn)) != SUCC)
		return r_type;

	for_pair(p, args) {
		if (!is_pair(cdr(p)) && cdr(p) != NULL) {
			*rtn = (struct pair *)alloc_err_msg("args list is not a list");
			return ERR_TYPE;
		}
		e = car(p);
		if (e && is_number(e)) {
			num = (struct number *)e;
			if (is_long(num)) {
				if (num->l_value > cur_lmax)
					cur_lmax = num->l_value;
				if (num->l_value < cur_lmin)
					cur_lmin = num->l_value;
			} else {
				if (num->d_value > cur_dmax)
					cur_dmax = num->d_value;
				if (num->d_value < cur_dmin)
					cur_dmin = num->d_value;
			}
		} else {
			*rtn = (struct pair *)alloc_err_msg("args is not number");
			return ERR_TYPE;
		}
	}

	if (cur_lmax < cur_dmax)
		n_max = alloc_double(cur_dmax);
	else
		n_max = alloc_long(cur_lmax);

	if (cur_lmin > cur_dmin)
		n_min = alloc_double(cur_dmin);
	else
		n_min = alloc_long(cur_lmin);

	*rtn = alloc_pair((struct exp *)n_min, (struct exp *)n_max);
	return SUCC;
}

enum rtn_type min(struct pair *args, struct exp **rtn) {
	enum rtn_type r_type;
	r_type = min_max(args, (struct pair **)rtn);
	if (r_type != SUCC)
		return r_type;
	else {
		*rtn = car((struct pair *)*rtn);
		return SUCC;
	}
}

enum rtn_type max(struct pair *args, struct exp **rtn) {
	enum rtn_type r_type;
	r_type = min_max(args, (struct pair **)rtn);
	if (r_type != SUCC)
		return r_type;
	else {
		*rtn = cdr((struct pair *)*rtn);
		return SUCC;
	}
}

/* type: 0 for =, 1 for >, 2 for <, 3 for >=, 4 for <= */
static enum rtn_type wrapper_for_less_more_equal(struct pair *args, struct exp **rtn, int type) {
	enum rtn_type r_type;
	int result;

	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
		return r_type;

	r_type = sub(args, rtn);
	if ((r_type = sub(args, rtn)) != SUCC)
		return r_type;

	switch (type) {
		case 0 :
			if (equal_zero((struct number *)*rtn)) result = 1;
			else result = 0;
			break;
		case 1 :
			if (above_zero((struct number *)*rtn)) result = 1;
			else result = 0;
			break;
		case 2 :
			if (below_zero((struct number *)*rtn)) result = 1;
			else result = 0;
			break;
		case 3 :
			if (above_zero((struct number *)*rtn) ||
					equal_zero((struct number *)*rtn))
				result = 1;
			else result = 0;
			break;
		case 4 :
			if (below_zero((struct number *)*rtn) ||
					equal_zero((struct number *)*rtn))
				result = 1;
			else result = 0;
			break;
		default :
			*rtn = (struct exp *)alloc_err_msg("template function:internal bug, unknow type");
			return ERR_TYPE;
	}

	*rtn = (struct exp *)alloc_bool(result);
	return SUCC;
}

enum rtn_type number_equal(struct pair *args, struct exp **rtn) {
	return wrapper_for_less_more_equal(args, rtn, 0);
}

enum rtn_type more_than(struct pair *args, struct exp **rtn) {
	return wrapper_for_less_more_equal(args, rtn, 1);
}

enum rtn_type less_than(struct pair *args, struct exp **rtn) {
	return wrapper_for_less_more_equal(args, rtn, 2);
}

enum rtn_type more_equal(struct pair *args, struct exp **rtn) {
	return wrapper_for_less_more_equal(args, rtn, 3);
}

enum rtn_type less_equal(struct pair *args, struct exp **rtn) {
	return wrapper_for_less_more_equal(args, rtn, 4);
}

enum rtn_type null_p(struct pair *args, struct exp **rtn) {
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	if (car(args) == NULL)
		*rtn = (struct exp *)alloc_bool(1);
	else
		*rtn = (struct exp *)alloc_bool(0);
	return SUCC;
}

static int is_procedure(struct exp *e) {
	return (is_callable(e) &&
			(is_lambda((struct callable *)e) ||
			is_builtin_pro((struct callable *)e)));
}

static enum rtn_type wrapper_for_type_p(struct pair *args, struct exp **rtn, int type) {
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;
	switch (type) {
		case NUMBER :
			if (is_number(car(args)))
				*rtn = (struct exp *)alloc_bool(1);
			else
				*rtn = (struct exp *)alloc_bool(0);
			break;
		case SYMBOL :
			if (is_symbol(car(args)))
				*rtn = (struct exp *)alloc_bool(1);
			else
				*rtn = (struct exp *)alloc_bool(0);
			break;
		case PAIR :
			if (is_pair(car(args)))
				*rtn = (struct exp *)alloc_bool(1);
			else
				*rtn = (struct exp *)alloc_bool(0);
			break;
		case CALLABLE :
			if (is_procedure(car(args)))
				*rtn = (struct exp *)alloc_bool(1);
			else
				*rtn = (struct exp *)alloc_bool(0);
			break;
		case BOOL :
			if (is_bool(car(args)))
				*rtn = (struct exp *)alloc_bool(1);
			else
				*rtn = (struct exp *)alloc_bool(0);
			break;
		default :
			*rtn = (struct exp *)alloc_err_msg("template function:internal bug, unknow type");
			return ERR_TYPE;
	}
	return SUCC;
}

enum rtn_type number_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_type_p(args, rtn, NUMBER);
}

enum rtn_type symbol_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_type_p(args, rtn, SYMBOL);
}

enum rtn_type pair_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_type_p(args, rtn, PAIR);
}

enum rtn_type procedure_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_type_p(args, rtn, CALLABLE);
}

enum rtn_type bool_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_type_p(args, rtn, BOOL);
}

enum rtn_type list_p(struct pair *args, struct exp **rtn) {
	struct exp *p;

	/* length require args to be list, we take advantage of this */
	if (length(args, &p) != SUCC)
		*rtn = (struct exp *)alloc_bool(0);
	else
		*rtn = (struct exp *)alloc_bool(1);
	return SUCC;
}

enum rtn_type not(struct pair *args, struct exp **rtn) {
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	if (is_bool(car(args)) && !is_true((struct bool *)car(args)))
		*rtn = (struct exp *)alloc_bool(1);
	else
		*rtn = (struct exp *)alloc_bool(0);

	return SUCC;
}

/* type 0 for positive, 1 for negative, 2 for zero_p */
static enum rtn_type wrapper_for_positive_negative_zero_p(struct pair *args,
		struct exp **rtn, int type) {
	int result;
	struct number *num;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	if (!is_number(car(args))) {
		*rtn = (struct exp *)alloc_err_msg("arg is not number");
		return ERR_TYPE;
	}
	num = (struct number *)car(args);
	switch (type) {
		case 0 :
			if (is_double(num))
				result = (num->d_value > 0);
			else if (is_long(num))
				result = (num->l_value > 0);
			break;
		case 1 :
			if (is_double(num))
				result = (num->d_value < 0);
			else if (is_long(num))
				result = (num->l_value < 0);
			break;
		case 2 :
			if (is_double(num))
				result = (num->d_value == 0);
			else if (is_long(num))
				result = (num->l_value == 0);
			break;
		default :
			*rtn = (struct exp *)alloc_err_msg("template function:internal bug, unknow type");
			return ERR_TYPE;
	}
	*rtn = (struct exp *)alloc_bool(result);
	return SUCC;
}

enum rtn_type positive_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_positive_negative_zero_p(args, rtn, 0);
}

enum rtn_type negative_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_positive_negative_zero_p(args, rtn, 1);
}

enum rtn_type zero_p(struct pair *args, struct exp **rtn) {
	return wrapper_for_positive_negative_zero_p(args, rtn, 2);
}

enum rtn_type odd_p(struct pair *args, struct exp **rtn) {
	int result;
	struct number *num;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	if (!is_number(car(args))) {
		*rtn = (struct exp *)alloc_err_msg("arg is not number");
		return ERR_TYPE;
	}

	num = (struct number *)car(args);
	if (!is_long(num)) {
		*rtn = (struct exp *)alloc_err_msg("arg is not integer");
		return ERR_TYPE;
	}
	result = num->l_value % 2;
	*rtn = (struct exp *)alloc_bool(result);
	return SUCC;
}

enum rtn_type even_p(struct pair *args, struct exp **rtn) {
	struct exp *tmp;
	struct pair *p;
	enum rtn_type r_type;
	if ((r_type = odd_p(args, &tmp) != SUCC))
		return r_type;

	p = alloc_pair(tmp, NULL);
	return not(p, rtn);
}

enum rtn_type eq_p(struct pair *args, struct exp **rtn) {
	int result = 0;
	struct exp *ar, *adr;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
		return r_type;

	ar = car(args);
	adr = car((struct pair *)cdr(args));
	if (ar == NULL ||
			is_pair(ar) ||
			is_callable(ar) ||
			is_bool(ar)) /* because boolean is static allocated */
		result = (ar == adr);
	else if (is_number(ar) && is_number(adr)) {
		struct number *num1, *num2;
		num1 = (struct number *)ar;
		num2 = (struct number *)adr;
		if (is_long(num1) && is_long(num2))
			result = (num1->l_value == num2->l_value);
		else if (is_double(num1) && is_double(num2))
			result = (num1->d_value == num2->d_value);
	} else if (is_symbol(ar) && is_symbol(adr)) {
		struct symbol *sym1, *sym2;
		sym1 = (struct symbol *)ar;
		sym2 = (struct symbol *)adr;
		result = !strcmp(sym1->sym, sym2->sym);
	}
	*rtn = (struct exp *)alloc_bool(result);
	return SUCC;
}

enum rtn_type u_print(struct pair *args, struct exp **rtn) {
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	print(stdout, car(args));
	printf("\n");
	*rtn = (struct exp *)&no_output_sym;
	return SUCC;
}

enum rtn_type u_abs(struct pair *args, struct exp **rtn) {
	struct exp *result;
	struct number *num;
	enum rtn_type r_type;

	if ((r_type = positive_p(args, &result)) != SUCC)
		return r_type;

	num = (struct number *)car(args);
	if (is_bool(result) && is_true((struct bool *)result))
		*rtn = (struct exp *)num;
	else {
		if (is_long(num))
			*rtn = (struct exp *)alloc_long(-(num->l_value));
		else if (is_double(num))
			*rtn = (struct exp *)alloc_double(-(num->d_value));
		else {
			*rtn = (struct exp *)alloc_err_msg("internal bug, unknow number type");
			return ERR_TYPE;
		}
	}
	return SUCC;
}

static enum rtn_type wrapper_for_floor_ceil(struct pair *args, struct exp **rtn, int type) {
	struct number *num;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;
	if (!is_number(car(args))) {
		*rtn = (struct exp *)alloc_err_msg("args is not number");
		return ERR_TYPE;
	}

	num = (struct number *)car(args);
	if (is_long(num))
		*rtn = (struct exp *)num;
	else if (is_double(num)) {
		if (!type)
			*rtn = (struct exp *)alloc_long(floor(num->d_value));
		else
			*rtn = (struct exp *)alloc_long(ceil(num->d_value));
	}

	return SUCC;
}

enum rtn_type u_floor(struct pair *args, struct exp **rtn) {
	return wrapper_for_floor_ceil(args, rtn, 0);
}

enum rtn_type u_ceil(struct pair *args, struct exp **rtn) {
	return wrapper_for_floor_ceil(args, rtn, 1);
}

static enum rtn_type dup(struct pair *orig, struct pair **head, struct exp ***tail) {
	struct pair *p;
	struct pair *head_p;
	struct exp **tail_p;
	struct pair *new;
	enum rtn_type r_type;

	tail_p = (struct exp **)&head_p;
	for_pair(p, orig) {
		if (!is_pair(cdr(p)) && cdr(p) != NULL) {
			*head = (struct pair *)alloc_err_msg("arg is not a list");
			return ERR_TYPE;
		}
		new = alloc_pair(car(p), NULL);
		*tail_p = (struct exp *)new;
		tail_p = &new->cdr;
		if (is_pair(car(p))) {
			/* here tail is useless */
			r_type = dup((struct pair *)car(p), (struct pair **)&new->car, tail);
			if (r_type != SUCC) {
				*head = (struct pair *)car(new);
				return r_type;
			}
		}
	}
	*head = head_p;
	*tail = tail_p;
	return SUCC;
}

enum rtn_type append(struct pair *args, struct exp **rtn) {
	struct exp *ar, *adr;
	struct pair *result;
	struct exp **last;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
		return r_type;

	ar = car(args);
	adr = car((struct pair *)cdr(args));
	if (!is_pair(ar)) {
		*rtn = (struct exp *)alloc_err_msg("arg is not a list");
		return ERR_TYPE;
	}
	r_type = dup((struct pair *)ar, &result, &last);
	if (r_type != SUCC) {
		*rtn = (struct exp *)result;
		return r_type;
	}

	*last = adr;
	*rtn = (struct exp *)result;
	return SUCC;
}



/* *
 * this eval is to exported to user space, it's just wrapper for eval
 * also, by definition, eval should be builtin_pro instead of builtin_syntax
 * but user_eval will use env to call eval, so we register user_eval as
 * builtin_syntax and eval it's args ourselves
 */
enum rtn_type user_eval(struct pair *args, struct exp **rtn, struct environ *env) {
	enum rtn_type r_type;
	struct exp *r_args;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	r_type = eval(car(args), &r_args, env);

	if (r_type != SUCC)
		return r_type;
	else
		return eval(r_args, rtn, env);
}

enum rtn_type u_if(struct pair *args, struct exp **rtn, struct environ *env) {
	enum rtn_type r_type;
	struct exp *predict;
	int nr_args;

	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC) {
		if ((r_type = check_args(args, 3, 0, rtn)) != SUCC)
			return r_type;
		else
			nr_args = 3;
	} else
		nr_args = 2;

	r_type = eval(car(args), &predict, env);
	if (r_type != SUCC)
		return r_type;

	if (is_bool(predict) && ((struct bool *)predict)->value == 0) {
		/* not true*/
		if (nr_args == 2) {
			*rtn = (struct exp *)alloc_err_msg("`if' requires 2 args");
			return ERR_ARGC;
		}
		return eval(car((struct pair *)cdr((struct pair *)cdr(args))), rtn, env);
	} else
		return eval(car((struct pair *)cdr(args)), rtn, env);
}

enum rtn_type define(struct pair *args, struct exp **rtn, struct environ *env) {
	struct exp *ar, *adr;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 1, rtn)) != SUCC)
		return r_type;

	ar = car(args);
	adr = car((struct pair *)cdr(args));
	if (is_symbol(ar)) { /* (define x 10) */
		if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
			return r_type;
		r_type = eval(adr, rtn, env);
		if (r_type != SUCC)
			return r_type;
		if (define_in_env(env, (struct symbol *)ar, *rtn)) {
			*rtn = (struct exp *)alloc_err_msg("could not define %s in env", ((struct symbol *)ar)->sym);
			return ERR_ENV;
		}
		*rtn = (struct exp *)&no_output_sym;
		return SUCC;
	} else if (is_pair(ar)) {
		/* (define (x y) y) */
		struct exp *aar, *dar, *dr;
		struct pair *pars, *body;
		struct symbol *s;
		struct pair *p;
		aar = car((struct pair *)ar);
		dar = cdr((struct pair *)ar);
		dr = cdr((struct pair *)args);
		if (!is_symbol(aar) ||
				!is_pair(dar) ||
				!is_pair(dr)) {
			*rtn = (struct exp *)alloc_err_msg("structure is not proper");
			return ERR_TYPE;
		}
		s = (struct symbol *)aar;
		pars = (struct pair *)dar;
		body = (struct pair *)dr;

		/* construct args for lambda */
		p = alloc_pair((struct exp *)pars, (struct exp *)body);
		r_type = lambda(p, rtn, env);
		if (r_type != SUCC)
			return r_type;
		if (define_in_env(env, s, *rtn)) {
			*rtn = (struct exp *)alloc_err_msg("could not define %s in env", s->sym);
			return ERR_ENV;
		}
		*rtn = (struct exp *)&no_output_sym;
		return SUCC;
	} else {
		*rtn = (struct exp *)alloc_err_msg("structure is not proper");
		return ERR_TYPE;
	}
}

enum rtn_type set(struct pair *args, struct exp **rtn, struct environ *env) {
	struct symbol *s;
	struct exp *ar;
	struct exp *adr;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
		return r_type;

	ar = car(args);
	adr = car((struct pair *)cdr(args));
	if (!is_symbol(ar)) {
		*rtn = (struct exp *)alloc_err_msg("requires symbol");
		return ERR_TYPE;
	}
	s = (struct symbol *)ar;

	r_type = eval(adr, rtn, env);
	if (r_type != SUCC)
		return r_type;

	if (set_in_env(env, s, *rtn)) {
		*rtn = (struct exp *)alloc_err_msg("could not set %s in env", s->sym);
		return ERR_ENV;
	}
	*rtn = (struct exp *)&no_output_sym;
	return SUCC;
}

enum rtn_type begin(struct pair *args, struct exp **rtn, struct environ *env) {
	struct pair *result;
	enum rtn_type r_type;
	if ((r_type = eval_sequence(args, env, &result)) == SUCC)
		return last_element(result, rtn);
	return r_type;
}

/* type 0 for lamdba, 1 for macro */
static enum rtn_type wrapper_for_lambda_defmacro(struct pair *args, struct exp **rtn, struct environ *env, int type) {
	struct pair *l_args;
	struct pair *l_body;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 1, rtn)) != SUCC)
		return r_type;

	if (is_pair(car(args)) || car(args) == NULL) /* we accept 0 args for lambda */
		l_args = (struct pair *)car(args);
	else {
		*rtn = (struct exp *)alloc_err_msg("structure is not proper");
		return ERR_TYPE;
	}

	l_body = (struct pair *)cdr(args); /* we know from check_args that a, rtnrgs should be list */

	if (type)
		*rtn = (struct exp *)alloc_macro(l_args, l_body, env);
	else
		*rtn = (struct exp *)alloc_lambda(l_args, l_body, env);
	return SUCC;
}


enum rtn_type lambda(struct pair *args, struct exp **rtn, struct environ *env) {
	return wrapper_for_lambda_defmacro(args, rtn, env, 0);
}

/* symtax of our defmacro just define a procedure: (defmacro (let args #!rest body) ...) */
enum rtn_type defmacro(struct pair *args, struct exp **rtn, struct environ *env) {
	enum rtn_type r_type;
	struct exp *ar, *body;
	struct symbol *name;
	struct pair *pars;
	struct pair *r_args;
	struct exp *macro;
	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
		return r_type;

	ar = car(args);
	body = car((struct pair *)cdr(args));
	if (!is_pair(ar) ||
			car((struct pair *)ar) == NULL ||
			!is_symbol(car((struct pair *)ar)) ||
			!is_pair(cdr((struct pair *)ar))) {
		*rtn = (struct exp *)alloc_err_msg("structure is not proper");
		return ERR_TYPE;
	}
	name = (struct symbol *)car((struct pair *)ar);
	pars = (struct pair *)cdr((struct pair *)ar);

	r_args = alloc_pair(body, NULL);
	r_args = alloc_pair((struct exp *)pars, (struct exp *)r_args);

	if ((r_type = wrapper_for_lambda_defmacro(r_args, &macro, env, 1)) != SUCC)
		return r_type;
	if (!is_callable(macro) || !is_macro((struct callable *)macro)) {
		*rtn = (struct exp *)alloc_err_msg("internal bug: wrapper return wrong value");
		return ERR_TYPE;
	}

	r_args = alloc_pair(macro, NULL);
	r_args = alloc_pair((struct exp *)name, (struct exp *)r_args);
	return define(r_args, rtn, env);
}

enum rtn_type macroexpand(struct pair *args, struct exp **rtn, struct environ *env) {
	/* we need env, so we pretend to be builtin_syntax */
	struct exp *ar, *dr;
	struct pair *p;
	struct pair *args_for_apply;
	enum rtn_type r_type;
	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;
	if ((r_type = eval(car(args), rtn, env)) != SUCC)
		return r_type;

	if (!is_pair(*rtn)) {
		*rtn = (struct exp *)alloc_err_msg("could not expand nonlist");
		return ERR_TYPE;
	}

	p = (struct pair *)*rtn;
	ar = car(p);
	dr = cdr(p);
	if ((r_type = eval(ar, rtn, env)) != SUCC) {
		*rtn = (struct exp *)alloc_err_msg("could not eval macro");
		return r_type;
	}
	if (!is_callable(*rtn) && !is_macro((struct callable *)*rtn)) {
		*rtn = (struct exp *)alloc_err_msg("not a macro");
		return ERR_TYPE;
	}

	/* construct args for apply */
	args_for_apply = alloc_pair(dr, NULL);
	args_for_apply = alloc_pair(*rtn, (struct exp *)args_for_apply);
	return apply(args_for_apply, rtn);
}

enum rtn_type quote(struct pair *args, struct exp **rtn, struct environ *env) {
	enum rtn_type r_type;
	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;
	*rtn = car(args);
	return SUCC;
}

enum rtn_type backquote(struct pair *args, struct exp **rtn, struct environ *env) {
	/* we search for unquote to eval it */
	struct exp *head = NULL;
	struct exp **tail = &head;

	struct pair *p;
	struct pair *new;
	struct exp *ar;
	struct exp *aar;
	struct exp *result;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0, rtn)) != SUCC)
		return r_type;

	if (!car(args)) {
		*rtn = NULL;
		return SUCC;
	}
	if (is_pair(car(args))) {
		args = (struct pair *)car(args);

		for_pair(p, args) {
			if (!is_pair(cdr(p)) && cdr(p) != NULL) {
				*rtn = (struct exp *)alloc_err_msg("args list is not a list");
				return ERR_TYPE;
			}
			ar = car(p);
			if (is_pair(ar)) {
				aar = car((struct pair *)ar);
				if (is_symbol(aar) && !strcmp(((struct symbol *)aar)->sym, "quotesplice")) {
					/* we can not use recursive to handler quotesplice */
					if (!is_pair(cdr((struct pair *)ar)) ||
							cdr((struct pair *)cdr((struct pair *)ar)) != NULL) {
						*rtn = (struct exp *)alloc_err_msg("arg of quotesplice is not a list");
						return ERR_TYPE;
					}
					r_type = eval(car((struct pair *)cdr((struct pair *)ar)), &result, env);
					if (r_type != SUCC) {
						*rtn = result;
						return r_type;
					}
					else if (!is_pair(result) && result != NULL) {
						*rtn = (struct exp *)alloc_err_msg("arg of quotesplice is not a list");
						return ERR_TYPE;
					}
					*tail = result;

					/* direct tail to the right place */
					struct pair *tmp;
					struct exp **tmp_tail;
					for_pair(tmp, (struct pair *)result) {
						if (!is_pair(cdr(tmp)) && cdr(tmp) != NULL) {
							*rtn = (struct exp *)alloc_err_msg("arg of quotesplice is not a list");
							return ERR_TYPE;
						}
						tmp_tail = &tmp->cdr;
					}
					tail = tmp_tail;
				} else {
					new = alloc_pair(ar, NULL); //makes call legal
					r_type = backquote(new, &result, env);
					if (r_type != SUCC) {
						*rtn = result;
						return r_type;
					}
					new = alloc_pair(result, NULL);
					*tail = (struct exp *)new;
					tail = (struct exp **)&new->cdr;
				}
			} else if (is_symbol(ar) && !strcmp(((struct symbol *)ar)->sym, "unquote")) {
				if (!is_pair(cdr(p)) ||
						cdr((struct pair *)cdr(p)) != NULL) {
					*rtn = (struct exp *)alloc_err_msg("arg of unquote is not a list");
					return ERR_TYPE;
				}
				r_type = eval(car((struct pair *)cdr(p)), &result, env);
				if (r_type != SUCC) {
					*rtn = result;
					return r_type;
				}
				*tail = result; //we may update tail, but as we will return, it makes no sense
				break;
			} else {
				new = alloc_pair(ar, NULL);
				*tail = (struct exp *)new;
				tail = (struct exp **)&new->cdr;
			}
		}
		*rtn = head;
	} else
		*rtn = car(args);
	return SUCC;
}

static struct environ *eval_map_base = NULL;
static struct stack *top = NULL;
/* *
 * ATTENTIOM: eval_map is not a thread-safe function, it use eval_map_base to
 * do the work.
 */
static enum rtn_type eval_map(struct exp *e, struct exp **rtn) {
	return eval(e, rtn, eval_map_base);
}

enum rtn_type eval_sequence(struct pair *args, struct environ *env, struct pair **rtn) {
	enum rtn_type r_type;
	push_stack(&top);
	top->env = eval_map_base;
	eval_map_base = env;

	r_type = map(eval_map, args, rtn);

	eval_map_base = top->env;
	pop_stack(&top);
	return r_type;
}

/* *
 * this function can not export to user space directly
 */
enum rtn_type eval(struct exp *e, struct exp **rtn, struct environ *env) {
	enum rtn_type type;
	if (e == NULL || is_number(e) || is_bool(e) || is_callable(e)) {
		*rtn = e;
		return SUCC;
	} else if (is_symbol(e)) {
		if (find_in_env(env, (struct symbol *)e, rtn))
			return SUCC;
		else {
			*rtn = (struct exp *)alloc_err_msg("unbond symbol %s", ((struct symbol *)e)->sym);
			return ERR_UNBOUND;
		}
	} else if (is_pair(e)) {
		struct pair *p = (struct pair *)e;
		struct exp *ar = car(p);
		if (is_symbol(ar) || is_pair(ar)) {
			type = eval(ar, rtn, env);
			if (type != SUCC)
				return type;

			if (is_callable(*rtn)) {
				struct exp *result;
				struct callable *pro = (struct callable *)*rtn;

				if (is_builtin_pro(pro) || is_lambda(pro) || is_macro(pro)) {
					struct pair *args_for_apply;
					if (!is_macro(pro)) {
						if ((type = eval_sequence((struct pair *)cdr(p), env, (struct pair **)rtn)) != SUCC)
							return type;
					} else
						*rtn = cdr(p);


					args_for_apply = alloc_pair(*rtn, NULL);
					args_for_apply = alloc_pair((struct exp *)pro, (struct exp *)args_for_apply);

					type = apply(args_for_apply, &result);
					if (type != SUCC) {
						*rtn = result;
						return type;
					}

					if (is_macro(pro))
						return eval(result, rtn, env); /* FIXME is env right? */
					else {
						*rtn = result;
						return type;
					}

				} else if (is_builtin_syntax(pro)) {
					type = pro->bs_value((struct pair *)cdr(p), rtn, env);
					if (type == SUCC)
						return SUCC;
					else
						return type;
				} else /* not implemented yet */
					return ERR_TYPE;
			} else /* !is_callable(*rtn) */
				return ERR_TYPE;
		} else /* !(is_symbol(ar) || is_pair(ar)) */
			return ERR_TYPE;
	} else
		return ERR_TYPE;
}

enum rtn_type apply(struct pair *args, struct exp **rtn) {
	struct environ *env;
	struct callable *c;
	struct exp *cadr;
	struct pair *r_args;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0, rtn)) != SUCC)
		return r_type;

	if (!is_callable(car(args)) || !is_pair(cdr(args))) {
		*rtn = (struct exp *)alloc_symbol("not callable or wrong arg");
		return ERR_TYPE;
	}

	cadr = car((struct pair *)cdr(args));
	if (!is_pair(cadr) && cadr != NULL) { /* allow 0 args */
		*rtn = (struct exp *)alloc_symbol("arg list is not a list");
		return ERR_TYPE;
	}

	c = (struct callable *)car(args);
	r_args = (struct pair *)cadr;

	if (is_lambda(c) || is_macro(c)) {
		env = extend_env(c->u_value.pars, r_args, c->u_value.bind);

		if (eval_sequence(c->u_value.body, env, (struct pair **)rtn) != SUCC)
			return ERR_USER_PRO;
		return last_element((struct pair *)*rtn, rtn);
	} else if (is_builtin_pro(c))
		return c->bp_value(r_args, rtn);
	else {
		*rtn = (struct exp *)alloc_symbol("internel bug, unknow callable object");
		return ERR_TYPE;
	}
}


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
