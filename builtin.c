#include "builtin.h"

enum rtn_type length(struct pair *args, struct exp **rtn) {
	unsigned int len = 0;
	struct pair *p;
	enum rtn_type r_type;
	struct exp *r_args;

	if ((r_type = check_args(args, 1, 0)) != SUCC)
		return r_type;

	r_args = car(args);

	if (!is_pair(r_args))
		return ERR_TYPE;
	for_pair(p, (struct pair *)r_args)
		len++;
	*rtn = (struct exp *)alloc_long(len);
	return SUCC;
}

enum rtn_type list(struct pair *args, struct exp **rtn) {
	*rtn = (struct exp *)args;
	return SUCC;
}

enum rtn_type cons(struct pair *args, struct exp **rtn) {
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0)) != SUCC)
		return r_type;

	*rtn = (struct exp *)alloc_pair(car(args), car((struct pair *)cdr(args)));
	return SUCC;
}

/* type 0 for car, 1 for cdr */
static enum rtn_type wrapper_for_car_cdr(struct pair *args, struct exp **rtn, int type) {
	struct exp *r_args;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 1, 0)) != SUCC)
		return r_type;
	r_args = car(args);
	if (is_pair(r_args)) {
		if (type == 0)
			*rtn = car((struct pair *)r_args);
		else
			*rtn = cdr((struct pair *)r_args);
		return SUCC;
	} else
		return ERR_TYPE;
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

	if ((r_type = check_args(args, 1, 1)) != SUCC)
		return r_type;

	for_pair(p, args) {
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
		} else
			return ERR_TYPE;
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
	struct pair *result;
	enum rtn_type r_type;
	r_type = min_max(args, &result);
	if (r_type != SUCC)
		return r_type;
	else {
		*rtn = car(result);
		return SUCC;
	}
}

enum rtn_type max(struct pair *args, struct exp **rtn) {
	struct pair *result;
	enum rtn_type r_type;
	r_type = min_max(args, &result);
	if (r_type != SUCC)
		return r_type;
	else {
		*rtn = cdr(result);
		return SUCC;
	}
}

/* type: 0 for =, 1 for >, 2 for <, 3 for >=, 4 for <= */
static enum rtn_type wrapper_for_less_more_equal(struct pair *args, struct exp **rtn, int type) {
	enum rtn_type r_type;
	int result;

	if ((r_type = check_args(args, 2, 0)) != SUCC)
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

/* *
 * this eval is to exported to user space, it's just wrapper for eval
 * also, by definition, eval should be builtin_pro instead of builtin_syntax
 * but user_eval will use env to call eval, so we register user_eval as
 * builtin_syntax and eval it's args ourselves
 */
enum rtn_type user_eval(struct pair *args, struct exp **rtn, struct environ *env) {
	enum rtn_type r_type;
	struct exp *r_args;

	if ((r_type = check_args(args, 1, 0)) != SUCC)
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

	if ((r_type = check_args(args, 2, 0)) != SUCC) {
		if ((r_type = check_args(args, 3, 0)) != SUCC)
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
		if (nr_args == 2)
			return ERR_ARGC;
		return eval(car((struct pair *)cdr((struct pair *)cdr(args))), rtn, env);
	} else
		return eval(car((struct pair *)cdr(args)), rtn, env);
}

enum rtn_type define(struct pair *args, struct exp **rtn, struct environ *env) {
	struct exp *ar, *adr;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 1)) != SUCC)
		return r_type;

	ar = car(args);
	adr = car((struct pair *)cdr(args));
	if (is_symbol(ar)) { /* (define x 10) */
		if ((r_type = check_args(args, 2, 0)) != SUCC)
			return r_type;
		r_type = eval(adr, rtn, env);
		if (r_type != SUCC)
			return r_type;
		if (define_in_env(env, (struct symbol *)ar, *rtn))
			return ERR_ENV;
		*rtn = NULL;
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
				!is_pair(dr))
			return ERR_TYPE;
		s = (struct symbol *)aar;
		pars = (struct pair *)dar;
		body = (struct pair *)dr;

		/* construct args for lambda */
		p = alloc_pair((struct exp *)pars, (struct exp *)body);
		r_type = lambda(p, rtn, env);
		if (r_type != SUCC)
			return r_type;
		if (define_in_env(env, s, *rtn))
			return ERR_ENV;
		*rtn = NULL;
		return SUCC;
	} else
		return ERR_TYPE;
}

enum rtn_type set(struct pair *args, struct exp **rtn, struct environ *env) {
	struct symbol *s;
	struct exp *ar;
	struct exp *adr;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0)) != SUCC)
		return r_type;

	ar = car(args);
	adr = car((struct pair *)cdr(args));
	if (!is_symbol(ar))
		return ERR_TYPE;
	s = (struct symbol *)ar;

	r_type = eval(adr, rtn, env);
	if (r_type != SUCC)
		return r_type;

	if (set_in_env(env, s, *rtn))
		return ERR_ENV;
	*rtn = NULL;
	return SUCC;
}

enum rtn_type begin(struct pair *args, struct exp **rtn, struct environ *env) {
	struct pair *result;
	enum rtn_type r_type;
	if ((r_type = eval_sequence(args, env, &result)) == SUCC)
		*rtn = last_element(result);
	return r_type;
}

enum rtn_type lambda(struct pair *args, struct exp **rtn, struct environ *env) {
	struct pair *l_args;
	struct pair *l_body;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 1)) != SUCC)
		return r_type;

	if (is_pair(car(args)) || car(args) == NULL) /* we accept 0 args for lambda */
		l_args = (struct pair *)car(args);
	else
		return ERR_TYPE;

	l_body = (struct pair *)cdr(args); /* we know from check_args that args should be list */

	*rtn = (struct exp *)alloc_lambda(l_args, l_body, env);
	return SUCC;
}

enum rtn_type quote(struct pair *args, struct exp **rtn, struct environ *env) {
	enum rtn_type r_type;
	if ((r_type = check_args(args, 1, 0)) != SUCC)
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

	if ((r_type = check_args(args, 1, 0)) != SUCC)
		return r_type;

	if (!car(args)) {
		*rtn = NULL;
		return SUCC;
	}
	if (is_pair(car(args))) {
		args = (struct pair *)car(args);

		for_pair(p, args) {
			ar = car(p);
			if (is_pair(ar)) {
				aar = car((struct pair *)ar);
				if (is_symbol(aar) && !strcmp(((struct symbol *)aar)->sym, "quotesplice")) {
					/* we can not use recursive to handler quotesplice */
					if (!is_pair(cdr((struct pair *)ar)) ||
							!is_pair(car((struct pair *)cdr((struct pair *)ar))) ||
							cdr((struct pair *)cdr((struct pair *)ar)) != NULL)
						return ERR_TYPE;
					r_type = eval(car((struct pair *)cdr((struct pair *)ar)), &result, env);
					if (r_type != SUCC)
						return r_type;
					else if (!is_pair(result))
						return ERR_TYPE;
					*tail = result;

					/* direct tail to the right place */
					struct pair *tmp;
					struct exp **tmp_tail;
					for_pair(tmp, (struct pair *)result)
						tmp_tail = &tmp->cdr;
					tail = tmp_tail;
				} else {
					new = alloc_pair(ar, NULL); //makes call legal
					r_type = backquote(new, &result, env);
					if (r_type != SUCC)
						return r_type;
					new = alloc_pair(result, NULL);
					*tail = (struct exp *)new;
					tail = (struct exp **)&new->cdr;
				}
			} else if (is_symbol(ar) && !strcmp(((struct symbol *)ar)->sym, "unquote")) {
				if (!is_pair(cdr(p)) ||
						cdr((struct pair *)cdr(p)) != NULL)
					return ERR_TYPE;
				r_type = eval(car((struct pair *)cdr(p)), &result, env);
				if (r_type != SUCC)
					return r_type;
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
