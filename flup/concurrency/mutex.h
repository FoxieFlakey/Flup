#ifndef UWU_C50E79D9_CA13_4623_A9A4_AF6F0401B7D6_UWU
#define UWU_C50E79D9_CA13_4623_A9A4_AF6F0401B7D6_UWU

/**
 * @file
 * @copydoc flup_mutex
 */

#include <pthread.h>

#include "flup/attributes.h"
#include "flup/thread/thread.h"

/**
 * @brief Mutual exclusion lock
 *
 * Its a structure which protects access to a resource
 * by only allowing one thread has access at anytime
 *
 * The clock used as reference is CLOCK_MONOTONIC
 */
typedef struct flup_mutex {
  /// Is this structure initialized
  bool initialized;
  /// The underlying pthread_mutex_t type for lock
  pthread_mutex_t lock;
  /// Thread which owns the mutex or NULL if none
  _Atomic(flup_thread*) owner;
} flup_mutex;

/**
 * @brief Allocate new mutex
 *
 * @return New instance of mutex or NULL on error
 */
FLUP_PUBLIC
flup_mutex* flup_mutex_new();

/**
 * @brief Free an allocated mutex
 *
 * @param self The mutex allocated from @ref flup_mutex_new
 */
FLUP_PUBLIC
void flup_mutex_free(flup_mutex* self);

/**
 * @brief Lock a mutex
 *
 * This may sleep indefinitely if the lock is locked until 
 * it is released
 *
 * @param self The mutex allocated from @ref flup_mutex_new
 */
FLUP_PUBLIC
void flup_mutex_lock(flup_mutex* self);

/**
 * @brief Unlock a mutex
 *
 * Unlock a locked mutex so its visible for other threads waiting
 *
 * @param self The mutex allocated from @ref flup_mutex_new
 */
FLUP_PUBLIC
void flup_mutex_unlock(flup_mutex* self);

#endif
