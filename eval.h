#ifndef EVAL_H
#define  EVAL_H

#include "env.h"
#include "types.h"

extern struct exp *eval(struct exp *e, struct environ *env);

#endif /*EVAL_H*/
