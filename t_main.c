#include "malloc.h"
#include "types.h"
#include "env.h"
#include "interpret.h"
#include "math.h"

/* *
 * this is the most basic scheme repl, compile it and run ./t_main
 * and then input something like '(+ 3 12 (* 32 2))', it will output 79, we
 * only support '+' and '*'
 */
int main(int argc, char *argv[]) {
	struct environ *base_env;
	struct symbol *s;
	struct callable *ca;

	base_env = alloc_environ(NULL);

	ca = alloc_builtin_pro(add);
	s = alloc_symbol("+");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(mul);
	s = alloc_symbol("*");
	define_in_env(base_env, s, (struct exp *)ca);

	interpret(stdin, stdout, stderr, base_env);

	exit(0);
}
