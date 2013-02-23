#include "builtin.h"

static enum rtn_type check_args(struct pair *args, unsigned int nr_arg) {
	unsigned int len = 0;
	struct pair *p;

	for_pair(p, args)
		len++;

	if (len != nr_arg)
		return ERR_ARGC;
	return SUCC;
}

enum rtn_type length(struct pair *args, struct exp **rtn) {
	unsigned int len = 0;
	struct pair *p;
	enum rtn_type r_type;
	struct exp *r_args;

	if ((r_type = check_args(args, 1)) != SUCC)
		return r_type;

	r_args = car(args);

	if (!is_pair(r_args))
		return ERR_TYPE;
	for_pair(p, (struct pair *)r_args)
		len++;
	*rtn = (struct exp *)alloc_long(len);
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

	if ((r_type = check_args(args, 1)) != SUCC)
		return r_type;

	r_type = eval(car(args), &r_args, env);

	if (r_type != SUCC)
		return r_type;
	else
		return eval(r_args, rtn, env);
}

enum rtn_type quote(struct pair *args, struct exp **rtn, struct environ *env) {
	enum rtn_type r_type;
	if ((r_type = check_args(args, 1)) != SUCC)
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

	if ((r_type = check_args(args, 1)) != SUCC)
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
						return ERR_ARG;
					r_type = eval(car((struct pair *)cdr((struct pair *)ar)), &result, env);
					if (r_type != SUCC)
						return r_type;
					else if (!is_pair(result))
						return ERR_ARG;
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
					return ERR_ARG;
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
