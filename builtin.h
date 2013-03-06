#ifndef BUILTIN_H
#define  BUILTIN_H

#include <string.h>
#include <float.h> /* get DBL_MAX */
#include <limits.h>
#include "types.h"
#include "eval.h"
#include "utils.h"
#include "malloc.h"
#include "math.h"

extern enum rtn_type length(struct pair *args, struct exp **rtn);
extern enum rtn_type list(struct pair *args, struct exp **rtn);
extern enum rtn_type cons(struct pair *args, struct exp **rtn);
extern enum rtn_type u_car(struct pair *args, struct exp **rtn);
extern enum rtn_type u_cdr(struct pair *args, struct exp **rtn);
extern enum rtn_type min(struct pair *args, struct exp **rtn);
extern enum rtn_type max(struct pair *args, struct exp **rtn);
extern enum rtn_type number_equal(struct pair *args, struct exp **rtn);
extern enum rtn_type more_than(struct pair *args, struct exp **rtn);
extern enum rtn_type less_than(struct pair *args, struct exp **rtn);
extern enum rtn_type more_equal(struct pair *args, struct exp **rtn);
extern enum rtn_type less_equal(struct pair *args, struct exp **rtn);
extern enum rtn_type user_eval(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type u_if(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type define(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type set(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type begin(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type lambda(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type defmacro(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type macroexpand(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type quote(struct pair *args, struct exp **rtn, struct environ *env);
extern enum rtn_type backquote(struct pair *args, struct exp **rtn, struct environ *env);

#endif /*BUILTIN_H*/
