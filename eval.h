#ifndef EVAL_H
#define  EVAL_H

#include "env.h"
#include "malloc.h"
#include "types.h"
#include "stack.h"
#include "apply.h"

extern enum rtn_type eval(struct exp *e, struct exp **rtn, struct environ *env);
extern enum rtn_type eval_sequence(struct pair *args, struct environ *env, struct pair **rtn);

#endif /*EVAL_H*/
