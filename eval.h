#ifndef EVAL_H
#define  EVAL_H

#include "env.h"
#include "types.h"

enum rtn_type eval(struct exp *e, struct exp **rtn, struct environ *env);

#endif /*EVAL_H*/
