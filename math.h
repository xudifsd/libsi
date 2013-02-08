#ifndef MATH_H
#define  MATH_H

#include "types.h"
#include "malloc.h"


extern enum rtn_type add(struct pair *args, struct exp **rtn);
extern enum rtn_type mul(struct pair *args, struct exp **rtn);

#endif /*MATH_H*/
