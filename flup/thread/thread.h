#ifndef UWU_E907D6A7_73B9_41EA_A0F0_F5492E8CDCDD_UWU
#define UWU_E907D6A7_73B9_41EA_A0F0_F5492E8CDCDD_UWU

/**
 * @file
 * @copydoc flup_thread
 */

#include <pthread.h>

/**
 * @brief A thread handle
 */
typedef struct flup_thread {
  /// Pthread handle to thread
  pthread_t thread;
} flup_thread;

#endif
