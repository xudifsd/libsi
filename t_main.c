#include "malloc.h"
#include "types.h"
#include "env.h"
#include "interpret.h"
#include "math.h"
#include "builtin.h"
#include "usage.h"
#include <stdio.h>

/* *
 * this is the most basic scheme repl, compile it and run ./t_main
 * and then input something like '(+ 3 12 (* 32 2))', it will output 79, we
 * only support '+' and '*'
 */
int main(int argc, char *argv[]) {
	struct environ *base_env;
	struct symbol *s;
	struct callable *ca;
	FILE *in;

	if (argc == 2) {
		in = fopen(argv[1], "r");
		if (!in)
			fatal(stderr, "Couldn't open %s", argv[1]);
	} else
		in = stdin;

	base_env = alloc_environ(NULL);

	ca = alloc_builtin_pro(add);
	s = alloc_symbol("+");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(mul);
	s = alloc_symbol("*");
	define_in_env(base_env, s, (struct exp *)ca);


	ca = alloc_builtin_pro(length);
	s = alloc_symbol("length");
	define_in_env(base_env, s, (struct exp *)ca);


	ca = alloc_builtin_syntax(user_eval);
	s = alloc_symbol("eval");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(quote);
	s = alloc_symbol("quote");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(backquote);
	s = alloc_symbol("backquote");
	define_in_env(base_env, s, (struct exp *)ca);

	interpret(in, stdout, stderr, base_env);

	exit(0);
}
