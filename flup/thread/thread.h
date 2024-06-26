#ifndef UWU_E907D6A7_73B9_41EA_A0F0_F5492E8CDCDD_UWU
#define UWU_E907D6A7_73B9_41EA_A0F0_F5492E8CDCDD_UWU

/**
 * @file
 * @copydoc flup_thread
 */

#include <pthread.h>
#include <threads.h>

#include "flup/attributes.h"
#include "flup/compiler.h"

/**
 * @brief A thread handle
 */
typedef struct flup_thread {
  /// Is the thread handle correspond attached thread
  /// not from flup_thread_new
  bool isThreadAttached;
  /// Is @ref flup_thread.thread initialized
  bool threadInitialized;
  /// Pthread handle to thread
  pthread_t thread;
  /// Name of the thread
  const char* name;
} flup_thread;

#if FLUP_APPLE_BLOCKS_AVAILABLE
/// The block pointer type for a thread routine
typedef void (^flup_thread_block)();

/**
 * @brief Create a new thread with a block
 * @public @memberof flup_thread
 *
 * This function creates a new thread with a block and
 * the block will be copied by Flup.
 *
 * @param block The block routine
 *
 * @return Thread instance if sucess or NULL on failure 
 */
FLUP_PUBLIC
flup_thread* flup_thread_new_with_block(flup_thread_block block);
#endif

/// The function pointer type for a thread routine
typedef void (*flup_thread_func)(void* udata);

/**
 * @brief Create a new thread with a function
 * @public @memberof flup_thread
 *
 * This function creates a new thread with a block and
 * the block is copied with Block_copy.
 *
 * @param func The function to be executed
 * @param udata Data to be passed as it is to thread's routine
 *
 * @return Thread instance if sucess or NULL on failure 
 */
FLUP_PUBLIC
flup_thread* flup_thread_new(flup_thread_func func, void* udata);

/**
 * @brief Frees the thread
 * @public @memberof flup_thread
 *
 * @warning Its undefined to free the thread while the thread running
 *   Flup won't make any attempt to kill nor stop the thread.
 *
 * @param self The thread to be free'd
 */
FLUP_PUBLIC
void flup_thread_free(flup_thread* self);

/**
 * @brief Wait for a thread termination
 * @public @memberof flup_thread
 *
 * @param self The thread to be waited on
 */
FLUP_PUBLIC
void flup_thread_wait(flup_thread* self);

/// Thread local pointer to current thread's handle (or NULL if unknown)
extern thread_local flup_thread* flup_current_thread;

/**
 * @brief Attach current thread so Flup know
 * @public @memberof flup_thread
 *
 * The thread will auto detach on thread exit.
 *
 * @param name The name of current thread
 *
 * @return New thread handle representing current thread, or NULL on error
 */
FLUP_PUBLIC
flup_thread* flup_attach_thread(const char* name);

/**
 * @brief Detachs current thread
 * @public @memberof flup_thread
 *
 * @return Current thread for cleaning up, or NULL if current isnt attached
 */
FLUP_PUBLIC
flup_thread* flup_detach_thread();

#endif
