#include <errno.h>
#include <stdint.h>

#include "flup/stacktrace/stacktrace.h"

#include "common.h"

int stacktrace_get_object(uintptr_t, flup_stacktrace_object*) {
  return -ENOSYS;
}

int stacktrace_get_source(uintptr_t, flup_stacktrace_source*) {
  return -ENOSYS;
}

int stacktrace_get_symbol(uintptr_t, flup_stacktrace_symbol*) {
  return -ENOSYS;
}

