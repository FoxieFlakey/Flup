#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "flup/core/panic.h"
#include "flup/attributes.h"

[[noreturn]]
FLUP_PUBLIC
void flup_panic(const char* format, ...) {
  va_list list;
  va_start(list, format);
  flup_vpanic(format, list);
  va_end(list);
}

[[noreturn]]
FLUP_PUBLIC
void flup_vpanic(const char* format, va_list list) {
  fprintf(stderr, "Panic occured -w-: ");
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
  vfprintf(stderr, format, list);
#ifdef __clang__
# pragma clang diagnostic pop
#endif
  fprintf(stderr, "\n");
  abort();
}

