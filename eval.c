#include "eval.h"
#include "utils.h"

static struct environ *eval_map_base;
/* *
 * ATTENTIOM: eval_map is not a thread-safe function, it use eval_map_base to
 * do the work.
 */
static inline struct exp *eval_map(struct exp *e) {
	return eval(e, eval_map_base);
}

struct exp *eval(struct exp *e, struct environ *env) {
	if (e == NULL)
		return NULL;
	else if (is_number(e))
		return e;
	else if (is_symbol(e)) {
		return find_in_env(env, (struct symbol *)e);
	} else if (is_pair(e)) {
		struct exp *val;
		struct pair *p = (struct pair *)e;
		val = find_in_env(env, (struct symbol *)car(p));
		if (!val)
			return NULL;
		if (is_callable(val)) {
			struct exp *rtn;
			struct callable *pro = (struct callable *)val;

			if (is_builtin_pro(pro)) {
				struct pair *args;
				eval_map_base = env;
				args = map(eval_map, (struct pair *)cdr(p)); /* eval args */

				pro->bp_value(args, &rtn);
				return rtn;
			} else {
				/* not implemented yet */
				return NULL;
			}
		} else {
			return NULL; /* Error */
		}
	} else
		return NULL; /* Error */
}
