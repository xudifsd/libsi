#ifndef TYPES_H
#define TYPES_H


struct environ {
	struct environ *parent;
	struct tree_node *repo;
};


#define NUMBER    (1<<0)
#define SYMBOL    (1<<1)
#define PAIR      (1<<2)
#define CALLABLE  (1<<3)
/* Maybe we will never support string type */


struct exp {
	unsigned long tag;
};

enum num_type {LONG, DOUBLE};

struct number {
	unsigned long tag;
	union {
		long l_value;
		double d_value;
	};
	enum num_type type;
};

#define SYMBOL_LEN 20

struct symbol {
	unsigned long tag;
	const char sym[SYMBOL_LEN];
};

struct pair {
	unsigned long tag;
	struct exp *car;
	struct exp *cdr;
};

/* we should ensure head is a list instead of like (1 . 2) */
#define for_pair(p, head) \
	for ((p) = (head); (p); (p) = (struct pair *)(p)->cdr)

enum callable_type {BUILTIN_SYNTAX, BUILTIN_PRO, LAMBDA, MACRO};

/* *
 * All the builtin functions and user functions that added directly
 * should have this prototype, which count contains the number of args
 * */

enum rtn_type {SUCC, ERR_ARGC, ERR_TYPE, ERR_ARG};

typedef enum rtn_type (*builtin_pro_f)(struct pair *args, struct exp **rtn);

typedef enum rtn_type (*builtin_syntax_f)(struct pair *args, struct exp **rtn, struct environ *env);

typedef struct lambda_s {
	struct pair *args;
	struct env *bind;
	struct pair *body;
} lambda_t;

typedef struct macro_s {
	struct pair *args;
	struct env *bind;
	struct pair *body;
} macro_t;

struct callable {
	unsigned long tag;
	union {
		builtin_pro_f bp_value;
		builtin_syntax_f bs_value;
		lambda_t l_value;
		macro_t m_value;
	};
	enum callable_type type;
};



/* This stack frame is *not* run-time stack,  it's parse-time stack */
struct stack_frame {
	struct stack_frame *prev;
	struct pair *head;
	struct pair **tail;
};

struct quote_stack {
	unsigned int nest;
	struct quote_stack *prev;
};


static inline int is_number(struct exp *var) {
	return (var && (var->tag & NUMBER));
}

static inline int is_long(struct number *num) {
	return (num && num->type == LONG);
}

static inline int is_double(struct number *num) {
	return (num && num->type == DOUBLE);
}

static inline int is_symbol(struct exp *var) {
	return (var && (var->tag & SYMBOL));
}

static inline int is_callable(struct exp *var) {
	return (var && (var->tag & CALLABLE));
}

static inline int is_pair(struct exp *var) {
	return (var->tag & PAIR);
}

static inline int is_builtin_syntax(struct callable *var) {
	return (var && (var->type == BUILTIN_SYNTAX));
}

static inline int is_builtin_pro(struct callable *var) {
	return (var && (var->type == BUILTIN_PRO));
}

static inline int is_lambda(struct callable *var) {
	return (var && (var->type == LAMBDA));
}

static inline int is_macro(struct callable *var) {
	return (var && (var->type == MACRO));
}
#endif /* TYPES_H */
