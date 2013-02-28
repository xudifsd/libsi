#ifndef EVAL_H
#define  EVAL_H

#include "env.h"
#include "malloc.h"
#include "types.h"
#include "apply.h"

extern enum rtn_type eval(struct exp *e, struct exp **rtn, struct environ *env);
extern struct pair *eval_sequence(struct pair *args, struct environ *env);

#endif /*EVAL_H*/
