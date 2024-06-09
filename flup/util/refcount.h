#ifndef UWU_EE5416D8_E140_4854_81A2_C0EC1608128B_UWU
#define UWU_EE5416D8_E140_4854_81A2_C0EC1608128B_UWU

#include <stdatomic.h>

/**
 * @file
 * @copydoc flup_refcount
 */

#include "flup/attributes.h"

/** 
 * @brief Fast atomic based refcount-ing
 *
 * Generate panic if counter overflow (possible leaks or 18 quadrillion usages exists)
 * , possible use after free (calling acquire when usage count is 0), or over release
 * (release when counter is 0 or below)
 */
typedef struct flup_refcount {
  /// Track number of usages
  atomic_uint_fast64_t usageCount;
} flup_refcount;

/**
 * @brief Initialize @ref flup_refcount
 *
 * @param self Refcount to be initialized
 */
FLUP_PUBLIC
void flup_refcount_init(flup_refcount* self);

/**
 * @public @memberof flup_refcount
 * @brief Increase usage count (acquire the thing)
 *
 * @param self Refcount to be incremented
 */
FLUP_PUBLIC
void flup_refcount_acquire(flup_refcount* self);

/**
 * @public @memberof flup_refcount
 * @brief Decrease usage count (release the thing)
 *
 * @param self Refcount to be decremented
 *
 * @return true if last reference is lost (so caller may free the resource)
 */
FLUP_PUBLIC
bool flup_refcount_release(flup_refcount* self);

#endif
