#ifndef header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h
#define header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h

#include <time.h>

#include "flup/attributes.h"

/**
 * @file
 * @copydoc flup_iqueue
 */

/**
 * @brief The generic iqueue structure
 * 
 * This queue interface, dequeue/enqueue a bare void* pointer.
 * @note Additional error code may exist depends on the implementation
 */
typedef struct flup_iqueue {
  /// The pointer to operations
  struct flup_iqueue_ops* ops;
} flup_iqueue;

/**
 * @brief Structure of operations supported by iqueue
 */
typedef struct flup_iqueue_ops {
  /**
   * @brief Get the length of queue
   * 
   * The length of items waiting in queue
   * @note Do not use this to conditionally dequeue/enqueue, use
   *   @ref dequeue or @ref enqueue with timeout at past instead.
   *
   * @param self The queue instance
   *
   * @return Length of items in queue
   */
  unsigned int (*getLength)(flup_iqueue* self);
  
  /**
   * @brief Enqueue an item to queue
   *
   * Enqueues an item with timeout or no timeout ( @p abstimeout is NULL)
   *
   * @param self The queue instance
   * @param item The item to be queued
   * @param abstime The timeout in absolute time 
   *
   * @throws EINVAL Invalid arguments
   * @throws ETIMEDOUT Time out reached (the queue is full for too long)
   *
   * @return 0 on sucess or -errno on error (see above)
   */
  int (*enqueue)(flup_iqueue* self, void* item, const struct timespec* abstimeout);
  
  /**
   * @brief Dequeues an item from queue
   *
   * Enqueues an item with timeout or no timeout ( @p abstimeout is NULL)
   *
   * @param self The queue instance
   * @param[out] item The result of dequeued item
   * @param abstime The timeout in absolute time 
   *
   * @throws EINVAL Invalid arguments
   * @throws ETIMEDOUT Time out reached (the queue is empty for too long)
   *
   * @return 0 on sucess or -errno on error (see above)
   */
  int (*dequeue)(flup_iqueue* self, void** item, const struct timespec* abstimeout);
  
  /**
   * @brief Deallocates the iqueue instance
   *
   * @param self The queue itself
   */
  void (*dealloc)(flup_iqueue* self);
} flup_iqueue_ops;

#endif

