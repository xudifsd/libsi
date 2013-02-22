#include "builtin.h"

enum rtn_type quote(struct pair *args, struct exp **rtn, struct environ *env) {
	if (cdr(args) != NULL)
		return ERR_ARGC;
	*rtn = (struct exp *)car(args);
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
	enum rtn_type type;

	if (cdr(args) != NULL)
		return ERR_ARGC;

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
					result = eval(car((struct pair *)cdr((struct pair *)ar)), env);
					if (!result || !is_pair(result))
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
					type = backquote(new, &result, env);
					if (type != SUCC)
						return type;
					new = alloc_pair(result, NULL);
					*tail = (struct exp *)new;
					tail = (struct exp **)&new->cdr;
				}
			} else if (is_symbol(ar) && !strcmp(((struct symbol *)ar)->sym, "unquote")) {
				if (!is_pair(cdr(p)) ||
						cdr((struct pair *)cdr(p)) != NULL)
					return ERR_ARG;
				result = eval(car((struct pair *)cdr(p)), env);
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
