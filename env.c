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

struct exp *find_in_env(struct environ *env, struct symbol *sym) {
	struct exp *rtn;
	struct environ *p;
	for (p = env; p; p = p->parent) {
		rtn = tree_find(p->repo, sym->sym);
		if (rtn)
			return rtn;
	}
	return NULL;
}

struct environ *extend_env(struct pair *pars, struct pair *args, struct environ *base) {
	struct pair *p1, *p2;
	struct exp *e1,  *e2;
	struct environ *rtn;

	rtn = alloc_environ(base);
	for (p1 = pars, p2 = args;
			p1 && p2;
			p1 = (struct pair *)cdr(p1),
				p2 = (struct pair *)cdr(p2)) {
		e1 = car(p1);
		e2 = car(p2);
		if (!is_symbol(e1))
			return NULL;
		define_in_env(rtn, (struct symbol *)e1, e2);
	}
	return rtn;
}
