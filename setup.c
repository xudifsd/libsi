#include "setup.h"

struct environ *setup_builtin() {
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

	ca = alloc_builtin_pro(length);
	s = alloc_symbol("length");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(list);
	s = alloc_symbol("list");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_log);
	s = alloc_symbol("log");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_sin);
	s = alloc_symbol("sin");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_cos);
	s = alloc_symbol("cos");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_tan);
	s = alloc_symbol("tan");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(cons);
	s = alloc_symbol("cons");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_car);
	s = alloc_symbol("car");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_cdr);
	s = alloc_symbol("cdr");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(min);
	s = alloc_symbol("min");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(max);
	s = alloc_symbol("max");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(apply);
	s = alloc_symbol("apply");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(sub);
	s = alloc_symbol("-");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(number_equal);
	s = alloc_symbol("=");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(more_than);
	s = alloc_symbol(">");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(less_than);
	s = alloc_symbol("<");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(more_equal);
	s = alloc_symbol(">=");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(less_equal);
	s = alloc_symbol("<=");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(null_p);
	s = alloc_symbol("null?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(number_p);
	s = alloc_symbol("number?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(symbol_p);
	s = alloc_symbol("symbol?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(pair_p);
	s = alloc_symbol("pair?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(procedure_p);
	s = alloc_symbol("procedure?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(bool_p);
	s = alloc_symbol("boolean?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(list_p);
	s = alloc_symbol("list?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(not);
	s = alloc_symbol("not");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(positive_p);
	s = alloc_symbol("positive?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(negative_p);
	s = alloc_symbol("negative?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(zero_p);
	s = alloc_symbol("zero?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(odd_p);
	s = alloc_symbol("odd?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(even_p);
	s = alloc_symbol("even?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(eq_p);
	s = alloc_symbol("eq?");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_read);
	s = alloc_symbol("read");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_print);
	s = alloc_symbol("print");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_abs);
	s = alloc_symbol("abs");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_floor);
	s = alloc_symbol("floor");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_ceil);
	s = alloc_symbol("ceiling");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(u_sqrt);
	s = alloc_symbol("sqrt");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_pro(append);
	s = alloc_symbol("append");
	define_in_env(base_env, s, (struct exp *)ca);



	ca = alloc_builtin_syntax(user_eval);
	s = alloc_symbol("eval");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(u_if);
	s = alloc_symbol("if");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(define);
	s = alloc_symbol("define");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(set);
	s = alloc_symbol("set!");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(begin);
	s = alloc_symbol("begin");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(lambda);
	s = alloc_symbol("lambda");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(defmacro);
	s = alloc_symbol("defmacro");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(macroexpand);
	s = alloc_symbol("macroexpand");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(quote);
	s = alloc_symbol("quote");
	define_in_env(base_env, s, (struct exp *)ca);

	ca = alloc_builtin_syntax(backquote);
	s = alloc_symbol("backquote");
	define_in_env(base_env, s, (struct exp *)ca);
	return base_env;
}

int setup_basic(struct environ *env) {
	FILE *null, *basic;
	null = fopen("/dev/null", "w");
	basic = fopen("/usr/local/share/si/setup.scm", "r");
	if (!null || !basic)
		return 1;

	interpret_file(basic, null, null, env);
	return 0;
}
