#ifndef UTILS_H
#define  UTILS_H

#include <stdio.h>
#include "types.h"

typedef enum rtn_type(*map_f)(struct exp *, struct exp **);

extern void print(FILE *port, struct exp *e);
extern enum rtn_type map(map_f fun, struct pair *args, struct pair **rtn);
extern enum rtn_type check_args(struct pair *args, unsigned int nr_arg, int at_least);
extern struct exp *last_element(struct pair *head);
extern int above_zero(struct number *num);
extern int below_zero(struct number *num);
extern int equal_zero(struct number *num);

static inline struct exp *car(struct pair *p) {
	return p->car;
}

static inline struct exp *cdr(struct pair *p) {
	return p->cdr;
}

#endif /*UTILS_H*/
