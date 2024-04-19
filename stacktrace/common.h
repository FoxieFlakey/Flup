#ifndef UWU_A92D67D3_5BCA_4032_A3BD_4134888D2BEB_UWU
#define UWU_A92D67D3_5BCA_4032_A3BD_4134888D2BEB_UWU

#include <stdint.h>

#include "flup/stacktrace/stacktrace.h"

int stacktrace_get_object(uintptr_t ip, flup_stacktrace_object* res);
int stacktrace_get_source(uintptr_t ip, flup_stacktrace_source* res);
int stacktrace_get_symbol(uintptr_t ip, flup_stacktrace_symbol* res);

#endif
