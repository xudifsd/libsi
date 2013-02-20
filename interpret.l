%option noyywrap

%{
#include <assert.h>
#include "interpret.h"

enum parse_type { PARSE_LB, PARSE_RB, PARSE_SYM, PARSE_LONG, PARSE_DOUBLE, PARSE_EOF};

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
[ \t\n]	{ /* skip */ }

<<EOF>>	{ return PARSE_EOF; }
%%

int interpret(FILE *in, FILE *out, FILE *err, struct environ *base_env) {
	yyin = in;
	enum parse_type rtn;
	struct exp *eval_result;

	struct pair *cur;
	struct pair *new;
	struct stack_frame *top = NULL;

	while (rtn != PARSE_EOF) {
		rtn = yylex();
		switch (rtn) {
			case PARSE_LB:
				push_stack(&top);
				break;
			case PARSE_RB:
				assert(top);
				cur = top->head;
				pop_stack(&top);

				if (top) {
					new = alloc_pair((struct exp *)cur, NULL);
					append_stack(top, new);
				} else {
					/* we are in top level */
					eval_result = eval((struct exp *)cur, base_env);
					print(out, eval_result);
					fputc('\n', out);
				}
				break;
			case PARSE_SYM:
			case PARSE_LONG:
			case PARSE_DOUBLE:
				if (top) {
					new = alloc_pair(yylval, NULL);
					append_stack(top, new);
				} else {
					eval_result = eval(yylval, base_env);
					print(out, eval_result);
					fputc('\n', out);
				}
				break;
			case PARSE_EOF:
				break;
		}
	}
	return 0;
}
