#pragma once

#include "types.h"
#include "eval.h"
#include "utils.h"
#include "env.h"

extern enum rtn_type apply(struct pair *args, struct exp **rtn);
