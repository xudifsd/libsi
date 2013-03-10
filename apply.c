#include "apply.h"

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
