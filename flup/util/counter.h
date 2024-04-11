#ifndef UWU_BA95A21A_D40F_45F8_92FF_932DE14FAAF9_UWU
#define UWU_BA95A21A_D40F_45F8_92FF_932DE14FAAF9_UWU

#include <stdint.h>
#include <stdatomic.h>

#include "flup/attributes.h"

/**
 * @file
 * @copydoc flup_counter
 */

/**
 * @brief Set and forget counter
 *
 * An 64-bit counter which counts from 1 to
 * `UINT64_MAX - 1` and paics when overflow (unlikely
 * on 64-bit integers). So you can set and forget
 * about it and keep getting the number.
 *
 * This never return number 0 and UINT64_MAX so
 * users of this can use those numbers as the
 * way they liked and they can be sure that
 * overflow won't cause damages (apart from
 * forced program abortions of course from panic)
 */
typedef struct flup_counter {
  /// The current counter
  atomic_uint_fast64_t counter;
} flup_counter;

/// Initializer for the counter
#define FLUP_COUNTER_INITIALIZER {.counter = 1}

/**
 * @brief Get current number and increment
 *
 * @param self The counter for the number
 * @return The current number
 */
FLUP_PUBLIC
uint64_t flup_counter_get(flup_counter* self);

#endif
