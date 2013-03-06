#pragma once

#include <math.h>
#include "types.h"
#include "malloc.h"
#include "utils.h"


extern enum rtn_type add(struct pair *args, struct exp **rtn);
extern enum rtn_type mul(struct pair *args, struct exp **rtn);
extern enum rtn_type u_log(struct pair *args, struct exp **rtn);
extern enum rtn_type u_sin(struct pair *args, struct exp **rtn);
extern enum rtn_type u_cos(struct pair *args, struct exp **rtn);
extern enum rtn_type u_tan(struct pair *args, struct exp **rtn);
extern enum rtn_type sub(struct pair *args, struct exp **rtn);
