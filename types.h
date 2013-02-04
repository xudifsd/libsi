#ifndef TYPES_H
#define TYPES_H

#include "stack.h"

/* *
 * All the builtin functions and user functions that added directly
 * should have this prototype, which count contains the number of args
 * */

enum rtn_type {SUCC, ERR_ARGC, ERR_TYPE};

typedef enum rtn_type (*builtin_t)(unsigned long argc, struct list *argv, struct exp **rtn);

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

enum callable_type {BUILTIN, LAMBDA, MACRO};

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
		builtin_t b_value;
		lambda_t l_value;
		macro_t m_value;
	};
	enum callable_type type;
};

static inline int is_number(struct exp *var) {
	return (var->tag & NUMBER);
}

static inline int is_symbol(struct exp *var) {
	return (var->tag & SYMBOL);
}

static inline int is_callable(struct exp *var) {
	return (var->tag & CALLABLE);
}

static inline int is_pair(struct exp *var) {
	return (var->tag & PAIR);
}

#endif /* TYPES_H */
