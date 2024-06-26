#ifndef UWU_A3FF6D1E_A4A7_4E6D_8A4C_FACF48082D8D_UWU
#define UWU_A3FF6D1E_A4A7_4E6D_8A4C_FACF48082D8D_UWU

#include <stddef.h>

#include "flup/util/iterator.h"

/**
 * @file
 * @copydoc flup_resetable_iterator
 */

typedef struct flup_resetable_iterator_ops flup_resetable_iterator_ops;

/**
 * @brief Resetable iterator state
 * @extends flup_iterator
 * 
 * Exactly the same as @ref flup_iterator non resetable
 * parent with addition of @ref flup_resetable_iterator.reset method
 */
typedef struct flup_resetable_iterator {
  /// The base for the non resetable stuffs
  flup_iterator super;
  /// Methods supported by resetable iterator
  const flup_resetable_iterator_ops* ops;
} flup_resetable_iterator;

/**
 * @brief The default iterator initialzer
 *
 * Use this to initialize as part of structure
 * initializer of the @ref flup_iterator
 */
#define FLUP_RESETABLE_ITERATOR_DEFAULTS_OPS .reset = NULL,

/**
 * @brief Contain operations supported by resettable iterator
 */
typedef struct flup_resetable_iterator_ops {
  /**
   * @brief Reset current iterator
   * @public @memberof flup_resetable_iterator
   *
   * Resets current iterator to start of iteration
   *
   * @note This method not allowed to set @ref flup_iterator.errorCode
   *   as this method can directly return the error code.
   *
   * @param self The iterator state
   *
   * @return 0 on success or -errno on error, the error codes depends on
   *   the implementation
   */
  int (*reset)(flup_resetable_iterator* self);
} flup_resetable_iterator_ops;

#endif
