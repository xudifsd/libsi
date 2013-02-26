#include "eval.h"
#include "utils.h"

static struct environ *eval_map_base;
/* *
 * ATTENTIOM: eval_map is not a thread-safe function, it use eval_map_base to
 * do the work.
 */
static inline struct exp *eval_map(struct exp *e) {
	struct exp *result;
	eval(e, &result, eval_map_base);
	/* FIXME we should check return value of eval and act according to it */
	return result;
}

/* *
 * this function can not export to user space directly
 */
enum rtn_type eval(struct exp *e, struct exp **rtn, struct environ *env) {
	enum rtn_type type;
	struct exp *value;
	if (e == NULL) {
		*rtn = NULL;
		return SUCC;
	} else if (is_number(e)) {
		*rtn = e;
		return SUCC;
	} else if (is_symbol(e)) {
		value = find_in_env(env, (struct symbol *)e);
		if (value) {
			*rtn = value;
			return SUCC;
		} else
			return ERR_UNBOUND;
	} else if (is_pair(e)) {
		struct exp *value;
		struct pair *p = (struct pair *)e;
		value = find_in_env(env, (struct symbol *)car(p));
		if (!value)
			return ERR_UNBOUND;
		if (is_callable(value)) {
			struct exp *result;
			struct callable *pro = (struct callable *)value;

			if (is_builtin_pro(pro)) {
				struct pair *args;
				eval_map_base = env;
				args = map(eval_map, (struct pair *)cdr(p)); /* eval args */

				type = pro->bp_value(args, &result);
				if (type == SUCC) {
					*rtn = result;
					return SUCC;
				}
				return type;
			} else if (is_builtin_syntax(pro)) {
				type = pro->bs_value((struct pair *)cdr(p), &result, env);
				if (type == SUCC) {
					*rtn = result;
					return SUCC;
				} else
					return type;
			} else {
				/* not implemented yet */
				return ERR_TYPE;
			}
		} else {
			return ERR_TYPE;
		}
	} else
		return ERR_TYPE;
}
