#ifndef header_1703857689_10eef286_2ea9_4689_a16e_c695ec779995_linked_list_h
#define header_1703857689_10eef286_2ea9_4689_a16e_c695ec779995_linked_list_h

/**
 * @file
 * @copydoc flup_linked_list
 */

#include <stddef.h>

#include "flup/data_structs/list_head.h"
#include "flup/interface/ilist.h"
#include "flup/util/iterator.h"
#include "flup/tags.h"
#include "flup/attributes.h"

/**
 * @brief The node of linked list
 */
typedef struct flup_linked_list_node {
  /// Node struct which contains next and prev
  flup_list_head node;
  /// The data stored by node
  char data[];
} flup_linked_list_node;

/**
 * @brief Simple doubly linked list
 *
 * A simple linked list using @ref flup_list_head.
 * Implements various interfaces useful for a list.
 */
typedef struct flup_linked_list {
  /// Interfaces implemeted
  struct {
    /// IList implementation
    flup_ilist IList;
  } interface;

  /// Length of the linked list
  size_t length;
  /// Size of each element
  size_t elementSize;
  
  /// The head of the linked list
  flup_list_head list;

  /**
   * @brief Version of linked list
   *
   * This incremented by 1 continuously
   * whenever a change which caused iteration
   * to behave incorrectly and cause the
   * iterator fail early. This implementation
   * will trigger @ref BUG on version mismatch
   * during iteration if bug check enabled.
   *
   * @note Do not rely on fail early to catch
   *   errors
   */
  int version;
} flup_linked_list;

/**
 * @brief Iterator of linked list
 */
typedef struct flup_linked_list_iterator {
  /// The struct which this derived from
  flup_iterator super;
  /// Pointer to which the iterator created from
  flup_linked_list* owner;
  /// Pointer to next item
  flup_linked_list_node* next;

  /**
   * @brief Version captured at time of creation
   *
   * The iterator compares this field with the
   * @ref flup_linked_list.version if not equal,
   * the iterator will trigger @ref BUG
   */
  int knownVersion;
} flup_linked_list_iterator;

/**
 * @brief Allocate new @ref flup_linked_list instance
 *
 * @param elementSize Size of each element
 * @throws NULL Out of memory while allocating
 * @return New instance of linked list or NULL (see above)
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_list* flup_linked_list_new(size_t elementSize);

/**
 * @brief Free a @ref flup_linked_list instance
 *
 * @param self Instance to be deallocated
 */
FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_linked_list_free(flup_linked_list* self);

/**
 * @brief Inserts an element to tail
 *
 * @param self Instance of @ref flup_linked_list
 * @param data The element to be inserted
 *
 * @throws NULL Out of memory while allocating new node
 *
 * @return New node for later reference or NULL (see above)
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_add_tail(flup_linked_list* self, const void* data);

/**
 * @brief Inserts an element to head
 *
 * @param self Instance of @ref flup_linked_list
 * @param[in] data The element to be inserted
 *
 * @throws NULL Out of memory while allocating new node
 *
 * @return New node for later reference or NULL (see above)
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_add_head(flup_linked_list* self, const void* data);

/**
 * @brief Initialize a new iterator
 *
 * Saves current version and initializes @a preAllocated
 * for iteration from head to tail.
 *
 * @param self Instance to be iterated
 * @param[out] preAllocated Caller allocated array for iterating
 */
FLUP_PUBLIC
void flup_linked_list_get_iterator(flup_linked_list* self, flup_linked_list_iterator* preAllocated);

/**
 * @brief Deletes a node
 *
 * Deletes a node and frees the node.
 *
 * @param self Instance of @ref flup_linked_list
 * @param node Node to be deleted
 */
FLUP_PUBLIC
void flup_linked_list_del(flup_linked_list* self, flup_linked_list_node* node);

/**
 * @brief Get element at index
 *
 * @param self Instance of @ref flup_linked_list
 * @param index Index of where the node is
 *
 * @throws NULL Index is out of bound
 *
 * @return A node at the index or NULL (see above)
 */
FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_at_index(flup_linked_list* self, size_t index);

#endif

