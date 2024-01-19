#ifndef header_1704106484_33446cea_a0ec_438b_b03d_f7196b444150_panic_h
#define header_1704106484_33446cea_a0ec_438b_b03d_f7196b444150_panic_h

#include <stdarg.h>

#include "flup/attributes.h"

FLUP_ATTR_PRINTF(1, 2)
[[noreturn]]
FLUP_PUBLIC
void flup_panic(const char* format, ...);

[[noreturn]]
FLUP_PUBLIC
void flup_vpanic(const char* format, va_list list);

#endif

