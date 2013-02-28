#include "apply.h"

enum rtn_type apply(struct pair *args, struct exp **rtn) {
	struct environ *env;
	struct callable *c;
	struct pair *result;
	struct pair *r_args;
	enum rtn_type r_type;

	if ((r_type = check_args(args, 2, 0)) != SUCC)
		return r_type;

	if (!is_callable(car(args)) || !is_pair(cdr(args)))
		return ERR_TYPE;

	if (!is_pair(car((struct pair *)cdr(args))))
		return ERR_TYPE;
	c = (struct callable *)car(args);
	r_args = (struct pair *)car((struct pair *)cdr(args));

	if (!is_lambda(c))
		return ERR_TYPE;
	env = extend_env(c->l_value.pars, r_args, c->l_value.bind);

	result = eval_sequence(c->l_value.body, env);
	*rtn = last_element(result);
	return SUCC;
}
