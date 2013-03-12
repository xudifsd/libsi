%option noyywrap yylineno

%{
#include "interpret.h"
#include "stack.h"
#include "usage.h"
#include "builtin.h"


enum parse_type { PARSE_LB, PARSE_RB, PARSE_SYM, PARSE_LONG, PARSE_DOUBLE,
	PARSE_EOF, PARSE_QUOTE, PARSE_BACKQUOTE, PARSE_UNQUOTE,
	PARSE_QUOTESPLICE, PARSE_TRUE, PARSE_FALSE, PARSE_DOT};

struct exp * yylval;
struct symbol no_output_sym = {SYMBOL, "no output"};
%}

%%
"("	{ return PARSE_LB; }
")"	{ return PARSE_RB; }
"."	{ return PARSE_DOT; }
[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {
	if (strchr(yytext, '.')) {
		struct number *d;
		d = alloc_double(atof(yytext));
		yylval = (struct exp *)d;
		return PARSE_DOUBLE;
	} else {
		struct number *l;
		l = alloc_long(atol(yytext));
		yylval = (struct exp *)l;
		return PARSE_LONG;
	}
}
#[tT] {
	struct bool *b;
	b = alloc_bool(1);
	yylval = (struct exp *)b;
	return PARSE_TRUE;
}
#[fF] {
	struct bool *b;
	b = alloc_bool(0);
	yylval = (struct exp *)b;
	return PARSE_FALSE;
}
[^',@` \(\)\t\n]+ {
	struct symbol *s;
	s = alloc_symbol(yytext);
	yylval = (struct exp *)s;
	return PARSE_SYM;
}
"'" { return PARSE_QUOTE; }
"`" { return PARSE_BACKQUOTE; }
"," { return PARSE_UNQUOTE; }
",@" { return PARSE_QUOTESPLICE; }
[ \t\n]	{ /* skip */ }
;.*$	{ /* skip comment */}

<<EOF>>	{ return PARSE_EOF; }
%%

static int is_no_output(struct exp *e) {
	struct symbol *sym;
	if (e && is_symbol(e)) {
		sym = (struct symbole *)e;
		if (sym == &no_output_sym)
			return 1;
	}
	return 0;
}

void eval_print(struct exp *in, FILE *out, FILE *err, struct environ *env, int lineno) {
	struct exp *result;
	enum rtn_type r_type;
	r_type = eval(in, &result, env);
	if (r_type == SUCC) {
		if (is_no_output(result))
			return;
		else {
			print(out, result);
			fputc('\n', out);
		}
	} else {
		error(err, "error in line %d: ", lineno);
		print(err, (struct exp *)in);
		fputc('\n', err);
		if (is_symbol(result))
			fprintf(err, "%s\n", ((struct symbol *)result)->sym);
		else
			fatal(err, "internal bug, type number is %d\n", result->tag);
	}
}

enum rtn_type u_read(struct pair *args, struct exp **rtn) {
	enum parse_type parse_rtn;
	enum rtn_type r_type;

	struct pair *cur;
	struct symbol *s;
	struct stack_frame *top = NULL;
	struct quote_stack *quote_top = NULL;
	struct dot_stack *dot_top = NULL;
	int dot_list = 0;

	if ((r_type = check_args(args, 0, 0, rtn)) != SUCC)
		return r_type;

	for (;;) {
		parse_rtn = yylex();
		switch (parse_rtn) {
			case PARSE_QUOTE:
				push_stack_frame(&top);
				s = alloc_symbol("quote");
				if (append_stack(top, (struct exp *)s, &dot_list)) {
					*rtn = (struct exp *)alloc_err_msg("error in append_stack, when append quote");
					return ERR_PARSE_DOT;
				}
				push_quote_stack(&quote_top);
				break;
			case PARSE_BACKQUOTE:
				push_stack_frame(&top);
				s = alloc_symbol("backquote");
				if (append_stack(top, (struct exp *)s, &dot_list)) {
					*rtn = (struct exp *)alloc_err_msg("error in append_stack, when append backquote");
					return ERR_PARSE_DOT;
				}
				push_quote_stack(&quote_top);
				break;
			case PARSE_UNQUOTE:
				push_stack_frame(&top);
				s = alloc_symbol("unquote");
				if (append_stack(top, (struct exp *)s, &dot_list)) {
					*rtn = (struct exp *)alloc_err_msg("error in append_stack, when append unquote");
					return ERR_PARSE_DOT;
				}
				push_quote_stack(&quote_top);
				break;
			case PARSE_QUOTESPLICE:
				push_stack_frame(&top);
				s = alloc_symbol("quotesplice");
				if (append_stack(top, (struct exp *)s, &dot_list)) {
					*rtn = (struct exp *)alloc_err_msg("error in append_stack, when append quotesplice");
					return ERR_PARSE_DOT;
				}
				push_quote_stack(&quote_top);
				break;
			case PARSE_LB:
				if (dot_list) {
					dot_list = 0;
					push_dot_stack(&dot_top);
					dot_top->dot_nest++;
				} else {
					if (dot_top)
						dot_top->dot_nest++;
					push_stack_frame(&top);
					if (quote_top)
						quote_top->nest++;
				}
				break;
			case PARSE_RB:
				if (dot_list) {
					*rtn = (struct exp *)alloc_err_msg("after . should have `)'");
					return ERR_PARSE_RB;
				}
				if (dot_top) {
					dot_top->dot_nest--;
					if (!dot_top->dot_nest) {
						pop_dot_stack(&dot_top);
						continue;
					}
				}
				if (!top) {
					*rtn = (struct exp *)alloc_err_msg("unexpected `)'");
					return ERR_PARSE_RB;
				} else {
					if (quote_top) {
						if (quote_top->nest)
							quote_top->nest--;
						while (quote_top && quote_top->nest == 0) {
							pop_quote_stack(&quote_top);

							cur = top->head;
							pop_stack_frame(&top);
							if (append_stack(top, (struct exp *)cur, &dot_list)) {
								*rtn = (struct exp *)alloc_err_msg(
										"error in append_stack, when PARSE_RB 1");
								return ERR_PARSE_DOT;
							}
						}
					}

					cur = top->head;
					pop_stack_frame(&top);

					if (top) {
						if (append_stack(top, (struct exp *)cur, &dot_list)) {
							*rtn = (struct exp *)alloc_err_msg(
									"error in append_stack, when PARSE_RB 2");
							return ERR_PARSE_DOT;
						}
					} else {
						*rtn = (struct exp *)cur;
						return SUCC;
					}
				}
				break;
			case PARSE_DOT:
				if (dot_list) {
					*rtn = (struct exp *)alloc_err_msg("invalid use of `.'");
					return ERR_PARSE_DOT;
				}
				if (!top) {
					*rtn = (struct exp *)alloc_err_msg("invalid use of `.'");
					return ERR_PARSE_DOT;
				}
				dot_list = 1;
				break;
			case PARSE_SYM: /* fall through */
			case PARSE_LONG: /* fall through */
			case PARSE_TRUE: /* fall through */
			case PARSE_FALSE: /* fall through */
			case PARSE_DOUBLE:
				if (top) {
					if (append_stack(top, yylval, &dot_list)) {
						*rtn = (struct exp *)alloc_err_msg(
								"error in append_stack, when append something");
						return ERR_PARSE_DOT;
					}

					cur = NULL;
					while (quote_top && quote_top->nest == 0) {
						pop_quote_stack(&quote_top);

						cur = top->head;
						pop_stack_frame(&top);
						if (top) {
							if (append_stack(top, (struct exp *)cur, &dot_list)) {
								*rtn = (struct exp *)alloc_err_msg(
										"error in append_stack, when append something in quote");
								return ERR_PARSE_DOT;
							}
						} else
							break;
					}
					if (!top) {
						*rtn = (struct exp *)cur;
						return SUCC;
					}
				} else {
					*rtn = yylval;
					return SUCC;
				}
				break;
			case PARSE_EOF:
				*rtn = (struct exp *)alloc_symbol("#!eof");
				return SUCC;
		}
	}
}

static int is_eof(struct exp *e) {
	if (is_symbol(e) && !strcmp(((struct symbol *)e)->sym, "#!eof"))
		return 1;
	else
		return 0;
}

void interpret_file(FILE *in, FILE *out, FILE *err, struct environ *base_env) {
	YY_BUFFER_STATE bp;
	struct exp *e;
	enum rtn_type r_type;
	FILE *null;
	if (!(null = fopen("/dev/null", "w")))
		fatal(err, "Could not open /dev/null");

	bp = yy_create_buffer(in, YY_BUF_SIZE);
	yy_switch_to_buffer(bp);
	for (;;) {
		r_type = u_read(NULL, &e);
		if (r_type != SUCC) {
			if (is_symbol(e)) {
				error(err, "error in line %d: %s\n", yylineno, ((struct symbol *)e)->sym);
				continue;
			} else
				fatal(err, "internal bug, e is not symbol");
		}
		if (is_eof(e))
			break;
		eval_print(e, null, err, base_env, yylineno);
	}
	yy_delete_buffer(bp);
}

void interpret_string(char *buf, FILE *out, FILE *err, struct environ *base_env) {
	YY_BUFFER_STATE bp;
	struct exp *e;
	enum rtn_type r_type;
	bp = yy_scan_string(buf);
	yy_switch_to_buffer(bp);
	for (;;) {
		r_type = u_read(NULL, &e);
		if (r_type != SUCC) {
			if (is_symbol(e)) {
				error(err, "error in line %d: %s\n", yylineno, ((struct symbol *)e)->sym);
				continue;
			} else
				fatal(err, "internal bug, e is not symbol");
		}
		if (is_eof(e))
			break;
		eval_print(e, out, err, base_env, yylineno);
	}
	yy_delete_buffer(bp);
}
