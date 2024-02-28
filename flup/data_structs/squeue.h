#ifndef header_1705058311_67951c3a_bf88_406f_89af_ffe6d430db65_flup_squeue_h
#define header_1705058311_67951c3a_bf88_406f_89af_ffe6d430db65_flup_squeue_h

/**
 * @file
 * @copydoc flup_squeue
 */

#include "flup/attributes.h"
#include "flup/compiler.h"
#include "flup/data_structs/list_head.h"
#include "flup/tags.h"

/**
 * @brief Simple queue structure
 * 
 * A simple queue implemented using linked list
 */
typedef struct flup_squeue {
  /// Maximum capacity the queue can be filled
  unsigned int capacity;
  /// Current length of queued items
  unsigned int length;
} flup_squeue;

/**
 * @brief An squeue item
 * 
 * The memory of this structure is managed
 * by the caller and provide the deallocation
 * function in @ref flup_squeue_item#dealloc
 */ 
typedef struct flup_squeue_item {
  /// The linked list node head
  flup_list_head node;
  /** 
   * @brief Dealloc current squeue item
   *
   * @param self The squeue item to be deallocated
   */
  void (*dealloc)(struct flup_squeue_item* self);
} flup_squeue_item;

/**
 * @brief A no-op item dealloc
 */
FLUP_USED
static void flup_squeue_nop_msg_dealloc(flup_squeue_item*) {
}

/**
 * @brief Initialize squeue item with defaults values
 */
#define FLUP_SQUEUE_ITEM_DEFAULTS .node = {}, .dealloc = flup_squeue_nop_msg_dealloc,

/**
 * @brief Allocate new squeue instance
 * 
 * Allocate a new squeue item which can filled by
 * @p capacity with @p flags
 *
 * @param capacity Maximum queue length
 * @param flags Flags indicate how queue will behave
 *   Pass zero for default behaviour
 *
 * @return Nonnull on success or NULL if out of memory
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_squeue* flup_squeue_new(unsigned int capacity, unsigned int flags);

/**
 * @brief Deallocate squeue
 * 
 * Deallocate squeue and also deallocates items
 * currently queue via @ref flup_squeue_item#dealloc
 * method.
 * 
 * @param self Instance of the queue to be deallocated
 */
FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_squeue_free(flup_squeue* self);

/**
 * @brief Enqueues an item
 * 
 * Enqueues @p item to @p self
 *
 * @param self Instance of squeue to enqueued to
 * @param item Instance of item to be queued
 *
 * @throws EOVERFLOW Queue is full
 * @throws EINVAL If @p item is NULL
 *
 * @return Zero on success or -errno, see above
 */
FLUP_PUBLIC
int flup_squeue_enqueue(flup_squeue* self, flup_squeue_item* item);

/**
 * @brief Dequeues an item
 *
 * Dequeues an item and return it.
 *
 * @param self Instance of squeue to be dequeued from
 *
 * @return The item currently at head or NULL if queue is empty
 */
FLUP_PUBLIC
flup_squeue_item* flup_squeue_try_dequeue(flup_squeue* self);

/**
 * @brief Filter function
 *
 * Check if current @p item is wanted item.
 *
 * @param item Item to be checked
 * @param udata Userdata passed as it is
 *
 * @return Return true if @p item is wanted one
 */
typedef bool (*flup_squeue_filter_func)(flup_squeue_item* item, void* udata);

/**
 * @brief Dequeue with a filter function
 *
 * Dequeue but with a filter to select which one to be dequeued
 *
 * @param self Instance of squeue to be dequeued from
 * @param filter The filter function
 * @param udata The userdata to be passed as it is to the filter function
 *
 * @return The item currently at head or NULL if queue is empty or none found
 */
FLUP_PUBLIC
flup_squeue_item* flup_squeue_dequeue_filtered_func(flup_squeue* self, flup_squeue_filter_func filter, void* udata);

/**
 * @brief An alias to corresponding filter dequeue func
 *
 * Expands to corresponding filter dequeue func. If Apple Blocks
 * available expands to @ref flup_squeue_dequeue_filtered_func else 
 * epxnads to @ref flup_squeue_dequeue_filtered_func.
 */
#define flup_squeue_dequeue_filtered flup_squeue_dequeue_filtered_func

#if FLUP_APPLE_BLOCKS_AVAILABLE
/**
 * @brief Filter block
 *
 * Check if current @p item is wanted item.
 *
 * @param item Item to be checked
 *
 * @return Return true if @p item is wanted one
 */
typedef bool (^flup_squeue_filter_block)(flup_squeue_item* msg);

/**
 * @brief Dequeue with a filter block
 *
 * Dequeue but with a filter to select which one to be dequeued
 *
 * @param self Instance of squeue to be dequeued from
 * @param filter The filter block
 *
 * @return The item currently at head or NULL if queue is empty or none found
 */
FLUP_PUBLIC
flup_squeue_item* flup_squeue_dequeue_filtered_blocks(flup_squeue* self, flup_squeue_filter_block filter);
# undef flup_squeue_dequeue_filtered
# define flup_squeue_dequeue_filtered flup_squeue_dequeue_filtered_blocks
#endif

#endif





