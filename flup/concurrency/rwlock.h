#ifndef UWU_FD25A50B_477A_4DC8_8E6D_670564861AF3_UWU
#define UWU_FD25A50B_477A_4DC8_8E6D_670564861AF3_UWU

/**
 * @file
 * @copydoc flup_rwlock
 */

#include <pthread.h>

#include "flup/attributes.h"

/**
 * @brief A read/write lock
 *
 * This lock allow reader to run concurrenctly while
 * writers can only run serially.
 */
typedef struct flup_rwlock {
  /// Pthread instance of rwlock
  pthread_rwlock_t rwlock;
  /// Is the rwlock initialized
  bool initialized;
} flup_rwlock;

/**
 * @brief Create new instance of rwlock
 *
 * @return New instance of rwlock or NULL on error
 */
FLUP_PUBLIC
flup_rwlock* flup_rwlock_new();

/**
 * @brief Frees an instance of rwlock
 *
 * @param self The rwlock instance to be freed
 */
FLUP_PUBLIC
void flup_rwlock_free(flup_rwlock* self);

/**
 * @brief Get reader lock
 *
 * @param self The rwlock instance to be locked on
 */
FLUP_PUBLIC
void flup_rwlock_rdlock(flup_rwlock* self);

/**
 * @brief Get writer lock
 *
 * @param self The rwlock instance to be locked on
 */
FLUP_PUBLIC
void flup_rwlock_wrlock(flup_rwlock* self);

/**
 * @brief Get unlocks the rwlock
 *
 * @param self The rwlock instance to be unlocked
 */
FLUP_PUBLIC
void flup_rwlock_unlock(flup_rwlock* self);

#endif
