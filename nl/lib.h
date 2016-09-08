#ifndef _LIB_HEADER_
#define _LIB_HEADER_

#include "define.h"
#include "util.h"
#include "gc.h"
#include "value.h"

#define LIB_FUNC(func_name) void func_name(VM* pvm, PValue params)

LIB_FUNC(Print);
LIB_FUNC(Write);
LIB_FUNC(SetMetaHash);
LIB_FUNC(Exit);

#endif
