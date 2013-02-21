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

				if (is_symbol(aar) &&
						(!strcmp(((struct symbol *)aar)->sym, "unquote") ||
						!strcmp(((struct symbol *)aar)->sym, "quotesplice"))) {
					result = eval((struct exp *)car((struct pair *)cdr((struct pair *)ar)), env);
					if (!result)
						return ERR_ARG;
				} else {
					new = alloc_pair(ar, NULL); //makes call legal
					type = backquote(new, &result, env);
					if (type != SUCC)
						return type;
				}
			} else
				result = ar;

			if (is_symbol(ar) && !strcmp(((struct symbol *)ar)->sym, "quotesplice"))
				*tail = (struct exp *)result;
			else {
				new = alloc_pair(result, NULL);
				*tail = (struct exp *)new;
				tail = (struct exp **)&new->cdr;
			}
		}
		*rtn = head;
	} else
		*rtn = car(args);
	return SUCC;
}
