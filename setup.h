#pragma once

#include "malloc.h"
#include "types.h"
#include "env.h"
#include "interpret.h"
#include "math.h"
#include "builtin.h"
#include "usage.h"

extern struct environ *setup_builtin();
