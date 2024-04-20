#ifndef header_1704106484_33446cea_a0ec_438b_b03d_f7196b444150_panic_h
#define header_1704106484_33446cea_a0ec_438b_b03d_f7196b444150_panic_h

/**
 * @file
 * @brief Panic functions
 *
 * Contains function for panic even where code
 * irrecoverably in erronous state.
 */

#include <stdarg.h>

#include "flup/attributes.h"

/**
 * @brief Trigger a panic and never return
 *
 * Prints formatted panic message to log and/or
 * stderr and then call abort().
 *
 * @param format Printf like format for panic message
 * @param ... Arguments to format message
 */
FLUP_ATTR_PRINTF(1, 2)
[[noreturn]]
FLUP_PUBLIC
void flup_panic(const char* format, ...);

/**
 * @brief Trigger a panic and never return
 *
 * Same as @ref panic but uses va_list
 *
 * @param format Printf like format for panic message
 * @param list va_list of arguments
 */
FLUP_ATTR_PRINTF(1, 0)
[[noreturn]]
FLUP_PUBLIC
void flup_vpanic(const char* format, va_list list);

#endif

