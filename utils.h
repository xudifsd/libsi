#ifndef UTILS_H
#define  UTILS_H

#include <stdio.h>
#include "types.h"

typedef struct exp *(*map_f)(struct exp *);

extern void print(FILE *port, struct exp *e);
extern struct pair *map(map_f fun, struct pair *p);
extern enum rtn_type check_args(struct pair *args, unsigned int nr_arg, int at_least);
extern struct exp *last_element(struct pair *head);

static inline struct exp *car(struct pair *p) {
	return p->car;
}

static inline struct exp *cdr(struct pair *p) {
	return p->cdr;
}

#endif /*UTILS_H*/
