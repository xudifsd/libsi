#include "env.h"

/* *
 * define only change current env, it's do not affect parent environ,
 * also, it do not replace old value
 */
int define_in_env(struct environ *env, struct symbol *sym, struct exp *value) {
	return tree_insert(&env->repo, sym, value, 0);
}

/* *
 * set_in_env is diff from define_in_env, we only replace old value in environ
 * hierach
 */
int set_in_env(struct environ *env, struct symbol *sym, struct exp *value) {
	struct environ *p;
	for (p = env; p; p = p->parent) {
		if (tree_insert(&p->repo, sym, value, 1))
			continue;
		return 0;
	}
	return 1;
}

/* return 1 for found, 0 for none */
int find_in_env(struct environ *env, struct symbol *sym, struct exp **rtn) {
	struct environ *p;
	for (p = env; p; p = p->parent) {
		if (tree_find(p->repo, sym->sym, rtn))
			return 1;
	}
	return 0;
}

struct environ *extend_env(struct pair *pars, struct pair *args, struct environ *base) {
	struct pair *p1, *p2;
	struct exp *e1, *e2;
	struct environ *rtn;

	rtn = alloc_environ(base);
	for (p1 = pars, p2 = args;
			p1;
			p1 = (struct pair *)cdr(p1),
				p2 = (struct pair *)cdr(p2)) {
		e1 = car(p1);
		if (!is_symbol(e1))
			return NULL;
		if (!strcmp(((struct symbol *)e1)->sym, "#!rest")) {
			p1 = (struct pair *)cdr(p1);
			e1 = car(p1);
			if (!is_symbol(e1) || cdr(p1) != NULL)
				return NULL;
			define_in_env(rtn, (struct symbol *)e1, (struct exp *)p2);
			break;
		}
		if (!p2)
			return NULL;
		e2 = car(p2);
		define_in_env(rtn, (struct symbol *)e1, e2);
	}
	return rtn;
}
