#ifndef UWU_D49C4473_4405_43B6_9BEB_F46D12F347FC_UWU
#define UWU_D49C4473_4405_43B6_9BEB_F46D12F347FC_UWU

#include <pthread.h>
#include <time.h>

#include "flup/attributes.h"
#include "flup/concurrency/mutex.h"

/**
 * @file
 * @copydoc flup_cond
 */

/**
 * @brief Condition
 *
 * Its a structure which allows application to signal
 * and wait for an event occur.
 *
 * The clock used as reference is CLOCK_REALTIME
 */
typedef struct flup_cond {
  /// Is this structure initalized
  bool initialized;
  /// The pthread's cond structure
  pthread_cond_t cond;
} flup_cond;

/**
 * @brief Statically define a preinitialized cond
 */
#define FLUP_COND_DEFINE(name) \
  flup_cond name = { \
    .initialized = true, \
    .cond = PTHREAD_COND_INITIALIZER \
  }

/**
 * @brief Statically define a statically linked preinitialized cond
 */
#define FLUP_COND_DEFINE_STATIC(name) \
  static flup_cond name = { \
    .initialized = true, \
    .cond = PTHREAD_COND_INITIALIZER \
  }

/**
 * @brief Create new instance of @ref flup_cond
 *
 * @return New instance or NULL on error
 */
FLUP_PUBLIC
flup_cond* flup_cond_new();

/**
 * @brief Free an instance of cond
 *
 * @param self The instance to be free'd
 */
FLUP_PUBLIC
void flup_cond_free(flup_cond* self);

/**
 * @public @memberof flup_cond
 * @brief Wake one waiting thread
 *
 * @param self The condition which the function will wake one 
 *   waiting thread from
 */
FLUP_PUBLIC
void flup_cond_wake_one(flup_cond* self);

/**
 * @public @memberof flup_cond
 * @brief Wake all waiting threads
 *
 * @param self The condition which the function will wake waiting
 *   threads from
 */
FLUP_PUBLIC
void flup_cond_wake_all(flup_cond* self);

/**
 * @public @memberof flup_cond
 * @brief Wait on a condition
 * 
 * Put current thread to sleep until its woken up by @ref flup_cond_wake_one
 * or @ref flup_cond_wake_all. Atomically unlock @p mutex before sleeping in
 * respect to other threads and relocks on wake up
 *
 * @note Wake up may happen even when nobody waking up the thread
 * @note Caller must already owned @p mutex when calling this function
 *
 * @param self The condition to wait on
 * @param mutex The mutex to be unlock and lock
 * @param abstimeout Absolute timeout while waiting, if NULL wait undefinitely
 *
 * @throws -ETIMEOUT Timeout reached, must not be returned if @p abstimeout
 *   is equal to NULL
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_cond_wait(flup_cond* self, flup_mutex* mutex, const struct timespec* abstimeout);

#endif
