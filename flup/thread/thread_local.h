#ifndef UWU_E890B422_81BD_4329_8EA3_3DB903674CC6_UWU
#define UWU_E890B422_81BD_4329_8EA3_3DB903674CC6_UWU

#include <pthread.h>
#include <stdint.h>

#include "flup/attributes.h"

/**
 * @file
 * @copydoc flup_thread_local
 */

/**
 * @brief The type of destructor function to be ran to destroy non zero
 *   @p val on each thread's instance if thread exits or the @ref flup_thread_local
 *   is destroyed
 *
 * @param val The value to be destroyed
 */
typedef void (*flup_thread_local_destructor)(uintptr_t val);

/** 
 * @brief Stores per-thread local uintptr_t value
 */
typedef struct flup_thread_local {
  /// Is this thread local initialized
  bool initialized;
  /// pthread_key_t of the thread local
  pthread_key_t key;
  /// Destructor function to destroy the value
  flup_thread_local_destructor destructor;
} flup_thread_local;

/**
 * @brief Create new thread local variable with a destructor
 *
 * @param destructor NULL or destructor function to be run to destroy the value
 *
 * @return New instance of thread local or NULL on out of memory
 */
FLUP_PUBLIC
flup_thread_local* flup_thread_local_new(flup_thread_local_destructor destructor);

/** 
 * @brief Free the thread local
 *
 * Frees the resources associated with thread local and run
 * destructor on all instances of per thread value
 *
 * @param self Thread local to be free'd
 */
FLUP_PUBLIC
void flup_thread_local_free(flup_thread_local* self);

/**
 * @public @memberof flup_thread_local
 * @brief Set value of thread local variable
 *
 * @param self Thread local to be set
 * @param newValue New value to be set with
 */
FLUP_PUBLIC
void flup_thread_local_set(flup_thread_local* self, uintptr_t newValue);

/**
 * @public @memberof flup_thread_local
 * @brief Get value of thread local variable
 *
 * @param self Thread local to get value from
 * 
 * @return Current thread local value
 */
FLUP_PUBLIC
uintptr_t flup_thread_local_get(flup_thread_local* self);

#endif
