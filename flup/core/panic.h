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
#include "flup/core/logger.h"

/**
 * @brief Trigger a panic and never return
 *
 * Prints formatted panic message to log and/or
 * stderr and then call abort().
 *
 * @param format Printf like format for panic message
 * @param ... Arguments to format message
 */
#define flup_panic(format, ...) do { \
  flup_logger__generate_callsite(callSite); \
  flup__panic(&callSite, (format) __VA_OPT__(,) __VA_ARGS__); \
} while (0)

/**
 * @brief Trigger a panic and never return
 *
 * Same as @ref flup_panic but uses va_list
 *
 * @param format Printf like format for panic message
 * @param list va_list of arguments
 */
#define flup_vpanic(format, list) do { \
  flup_logger__generate_callsite(callSite); \
  flup__vpanic(&callSite, (format), list); \
} while (0)

/// @cond 0
FLUP_ATTR_PRINTF(2, 3)
[[noreturn]]
FLUP_PUBLIC
void flup__panic(const struct printk_call_site_info* site, const char* format, ...);

FLUP_ATTR_PRINTF(2, 0)
[[noreturn]]
FLUP_PUBLIC
void flup__vpanic(const struct printk_call_site_info* site, const char* format, va_list list);
/// @endcond

#endif

