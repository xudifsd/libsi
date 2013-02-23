%option noyywrap

%{
#include <assert.h>
#include "interpret.h"
#include "stack.h"
#include "usage.h"
#include "eval.h"


enum parse_type { PARSE_LB, PARSE_RB, PARSE_SYM, PARSE_LONG, PARSE_DOUBLE,
	PARSE_EOF, PARSE_QUOTE, PARSE_BACKQUOTE, PARSE_UNQUOTE,
	PARSE_QUOTESPLICE};

struct exp * yylval;
%}

%%
"("	{ return PARSE_LB; }
")"	{ return PARSE_RB; }
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

void eval_print(struct exp *in, FILE *out, FILE *err, struct environ *env, int lineno) {
	struct exp *result;
	enum rtn_type r_type;
#ifndef DEBUG
	r_type = eval(in, &result, env);
	if (r_type == SUCC) {
		print(out, result);
		fputc('\n', out);
	} else {
		error(err, "error in line %d:", lineno);
		print(err, (struct exp *)in);
		fputc('\n', err);
	}
#else
	print(out, in);
	fputc('\n', out);
#endif
}

int interpret(FILE *in, FILE *out, FILE *err, struct environ *base_env) {
	yyin = in;
	enum parse_type rtn;

	struct pair *cur;
	struct symbol *s;
	struct stack_frame *top = NULL;
	struct quote_stack *quote_top = NULL;

	while (rtn != PARSE_EOF) {
		rtn = yylex();
		switch (rtn) {
			case PARSE_QUOTE:
				push_stack(&top);
				s = alloc_symbol("quote");
				append_stack(top, (struct exp *)s);
				push_quote_stack(&quote_top);
				break;
			case PARSE_BACKQUOTE:
				push_stack(&top);
				s = alloc_symbol("backquote");
				append_stack(top, (struct exp *)s);
				push_quote_stack(&quote_top);
				break;
			case PARSE_UNQUOTE:
				push_stack(&top);
				s = alloc_symbol("unquote");
				append_stack(top, (struct exp *)s);
				push_quote_stack(&quote_top);
				break;
			case PARSE_QUOTESPLICE:
				push_stack(&top);
				s = alloc_symbol("quotesplice");
				append_stack(top, (struct exp *)s);
				push_quote_stack(&quote_top);
				break;

			case PARSE_LB:
				push_stack(&top);
				if (quote_top)
					quote_top->nest++;
				break;
			case PARSE_RB:
				assert(top);

				if (quote_top) {
					if (quote_top->nest)
						quote_top->nest--;
					while (quote_top && quote_top->nest == 0) {
						pop_quote_stack(&quote_top);

						cur = top->head;
						pop_stack(&top);
						append_stack(top, (struct exp *)cur);
					}
				}

				cur = top->head;
				pop_stack(&top);

				if (top) {
					append_stack(top, (struct exp *)cur);
				} else
					eval_print((struct exp *)cur, out, err, base_env, yylineno);
				break;
			case PARSE_SYM: /* fall through */
			case PARSE_LONG: /* fall through */
			case PARSE_DOUBLE:
				cur = NULL;
				if (top) {
					append_stack(top, yylval);

					cur = NULL;
					while (quote_top && quote_top->nest == 0) {
						pop_quote_stack(&quote_top);

						cur = top->head;
						pop_stack(&top);
						if (top) {
							append_stack(top, (struct exp *)cur);
						} else
							break;
					}
					if (!top) {
						assert(cur);
						eval_print((struct exp *)cur, out, err, base_env, yylineno);
					}
				} else {
					/* we are in top level */
					if (cur)
						eval_print((struct exp *)cur, out, err, base_env, yylineno);
					else
						eval_print((struct exp *)yylval, out, err, base_env, yylineno);
				}
				break;
			case PARSE_EOF:
				break;
		}
	}
	return 0;
}
