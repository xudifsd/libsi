#include "eval.h"
#include "utils.h"

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
		else
			return ERR_UNBOUND;
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
					struct pair *args;
					struct pair *args_for_apply;
					if ((type = eval_sequence((struct pair *)cdr(p), env, &args)) != SUCC)
						return type;

					args_for_apply = alloc_pair((struct exp *)args, NULL);
					args_for_apply = alloc_pair((struct exp *)pro, (struct exp *)args_for_apply);

					type = apply(args_for_apply, &result);
					if (type != SUCC)
						return type;

					if (is_macro(pro))
						return eval(result, rtn, env); /* FIXME is env right? */
					else {
						*rtn = result;
						return type;
					}

				} else if (is_builtin_syntax(pro)) {
					type = pro->bs_value((struct pair *)cdr(p), &result, env);
					if (type == SUCC) {
						*rtn = result;
						return SUCC;
					} else
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
