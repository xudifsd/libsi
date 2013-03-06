#include "apply.h"

enum rtn_type apply(struct pair *args, struct exp **rtn) {
	struct environ *env;
	struct callable *c;
	struct exp *cadr;
	struct pair *result;
	struct pair *r_args;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0)) != SUCC)
		return r_type;

	if (!is_callable(car(args)) || !is_pair(cdr(args)))
		return ERR_TYPE;

	cadr = car((struct pair *)cdr(args));
	if (!is_pair(cadr) && cadr != NULL) /* allow 0 args */
		return ERR_TYPE;

	c = (struct callable *)car(args);
	r_args = (struct pair *)cadr;

	if (is_lambda(c) || is_macro(c)) {
		env = extend_env(c->u_value.pars, r_args, c->u_value.bind);

		if (eval_sequence(c->u_value.body, env, &result) != SUCC)
			return ERR_LAMBDA;
		*rtn = last_element(result);
		return SUCC;
	} else if (is_builtin_pro(c))
		return c->bp_value(r_args, rtn);
	else
		return ERR_TYPE;
}
