#ifndef BUILTIN_H
#define  BUILTIN_H

#include <string.h>
#include "types.h"
#include "eval.h"
#include "utils.h"
#include "malloc.h"

extern enum rtn_type length(struct pair *args, struct exp **rtn);
extern enum rtn_type quote(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type backquote(struct pair *args, struct exp **rtn, struct environ *env);

#endif /*BUILTIN_H*/
