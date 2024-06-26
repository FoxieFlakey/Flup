#ifndef header_1705806604_1d522638_a3ed_4c40_bf4c_0ccec5043563_list_h
#define header_1705806604_1d522638_a3ed_4c40_bf4c_0ccec5043563_list_h

/**
 * @file
 * @copydoc flup_ilist
 */

#include <stddef.h>

#include "flup/attributes.h"
#include "flup/util/iterator.h"
#include "flup/util/iterator_resetable.h"

/**
 * @brief (Interface) Generic IList
 *
 * Its a structure embedded in implementor's structure.
 */
typedef struct flup_ilist {
  /// Contains pointer to implemented methods
  const struct flup_ilist_ops* ops;
} flup_ilist;

/**
 * @brief A struct containing different operations
 *
 * @note Additional error not in here may be thrown by
 *   implementation
 */
typedef struct flup_ilist_ops {
  /**
   * @brief Returns length of IList
   * @public @memberof flup_ilist 
   * 
   * @param self Pointer to @ref flup_ilist
   *
   * @return length Length of the IList
   */
  size_t (*length)(flup_ilist* self);
  
  /**
   * @brief Return size of each element
   * @public @memberof flup_ilist 
   * 
   * The size of elements allow caller at runtime allocate
   * necessary memory without knowing the type.
   *
   * @param self Pointer to @ref flup_ilist
   * 
   * @return Return size of each element 
   */
  size_t (*elementSize)(flup_ilist* self);
  
  /**
   * @brief Insert an element at @p index
   * @public @memberof flup_ilist 
   * 
   * @param self Pointer to @ref flup_ilist
   * @param index Index to be inserted to
   * @param element Pointer to element to be inserted
   *
   * Non exhaustive list of errors (as it depends on the
   * implementation of what be thrown)
   *
   * @throws ENOMEM Not enough memory
   * @throws EINVAL Invalid @p index or @p element is NULL
   *
   * @return 0 on success, or -errno on error
   */
  int (*insert)(flup_ilist* self, size_t index, const void* element);
  
  /**
   * @brief Removes an element at @p index by @p count
   * @public @memberof flup_ilist 
   *
   * Non exhaustive list of errors (as it depends on the
   * implementation of what be thrown)
   * 
   * @throws EINVAL Invalid @p index or @p element is NULL
   *
   * @return 0 on success, or -errno on error
   */
  int (*remove)(flup_ilist* self, size_t index, size_t count);
  
  /**
   * @brief Get an item
   * @public @memberof flup_ilist 
   *
   * Gets an item at @p index and saves the pointer to it
   * to @p element.
   *
   * @note It is unsafe when the list modified such a way
   *   that the pointer is no longer valid therefore its
   *   required to avoid modifying the list (add, remove, etc).
   *
   * @note If non failable required consider @ref tryGet(list)
   *   which treats error as index doesnt exist
   *
   * @param self The instance of list
   * @param index The index of where the element retrieved
   * @param[out] element Pointer to void* where the result be
   *   saved
   *
   * @return true if exist and false if not exist, else -errno
   *   on error (the error depends on the implementation).
   */
  int (*get)(flup_ilist* self, size_t index, void** element);
  
  /**
   * @brief Deallocs @ref flup_ilist dealloc
   * @public @memberof flup_ilist 
   * 
   * @param self The instance to dealloc
   */
  void (*dealloc)(flup_ilist* self);
  
  /**
   * @brief (With default) Append to list
   * @public @memberof flup_ilist 
   *
   * The default implementation is to @ref flup_ilist.insert
   * with index at the end of list
   *
   * @param self The instance of self to insert @p element into
   * @param element The element to be inserted
   *
   * @throws ENOMEM Not enough memory
   * @throws EINVAL Invalid @p element is NULL
   *
   * @return 0 on sucess, else -errno on error (see above)
   */
  int (*append)(flup_ilist* self, const void* element);
  
  /**
   * @brief (With default) Prepends element to list
   * @public @memberof flup_ilist 
   *
   * The default implementation is to @ref flup_ilist.insert
   * with index at the front of list
   *
   * @param self The instance of self to insert @p element into
   * @param element The element to be inserted
   *
   * @throws ENOMEM Not enough memory
   * @throws EINVAL Invalid @p element is NULL
   *
   * @return 0 on sucess, else -errno on error (see above)
   */
  int (*prepend)(flup_ilist* self, const void* element);
  
  /**
   * @brief (With default) Get a @ref flup_iterator
   * @public @memberof flup_ilist 
   *
   * The default implementation is using @ref flup_ilist.get
   * to iterate by tracking the index.
   *
   * @param self The instance of @ref flup_ilist to be iterated
   *
   * @throw NULL Out of memory or any other error
   *
   * @return 0 on sucess, else NULL on error (see above)
   */
  flup_iterator* (*getIterator)(flup_ilist* self);
  
  /**
   * @brief (With default) Try get an element
   * @public @memberof flup_ilist 
   *
   * Gets an element and return true if exist and false if
   * not exist. The default implementation is using @ref flup_ilist.get
   * and return true if thats returns true otherwise false (errors
   * or actually don't exist)
   *
   * @param self The instance of list
   * @param index The index of where the element retrieved
   * @param[out] element Pointer to void* where the item pointer be
   *   saved
   *
   * @return true if exist, false if not exist or error occured
   */
  bool (*tryGet)(flup_ilist* self, size_t index, void** element);
} flup_ilist_ops;

/**
 * @copydoc flup_ilist.append
 * @protected @memberof flup_ilist
 */
FLUP_PUBLIC
int flup_impl_ilist_append(flup_ilist* self, const void* element);

/**
 * @copydoc flup_ilist.prepend
 * @protected @memberof flup_ilist
 */
FLUP_PUBLIC
int flup_impl_ilist_prepend(flup_ilist* self, const void* element);

/**
 * @copydoc flup_ilist.getIterator
 * @protected @memberof flup_ilist
 * @see Default iterator is @ref flup_ilist_iterator
 */
FLUP_PUBLIC
flup_iterator* flup_impl_ilist_get_iterator(flup_ilist* self);

/**
 * @copydoc flup_ilist.tryGet
 * @protected @memberof flup_ilist
 */
FLUP_PUBLIC
bool flup_impl_ilist_try_get(flup_ilist* self, size_t index, void** element);

/**
 * @brief Contains default @ref flup_ilist initializers
 *
 * This should be initialize the @ref flup_ilist first like
 *
 * ```c
 * struct app_ilist_ops = { FLUP_ILIST_OPS_DEFAULTS
 *   // <app's own implementations>
 * }
 * ```
 *
 * To ensure forward compatibility of the source code
 */
#define FLUP_ILIST_OPS_DEFAULTS \
  .append = flup_impl_ilist_append, \
  .prepend = flup_impl_ilist_prepend, \
  .getIterator = flup_impl_ilist_get_iterator, \
  .tryGet = flup_impl_ilist_try_get,

/**
 * @brief IList default iterator structure
 * @extends flup_resetable_iterator
 */
typedef struct flup_ilist_iterator {
  /// The base structure of iterator
  flup_resetable_iterator super;
  /// Which IList this iterator belongs to
  flup_ilist* owner;

  /**
   * @brief Containing index to next
   *
   * Containg index to next item, maybe invalid check if next
   * is present or not with @ref flup_ilist_iterator.hasNext
   */
  size_t nextIndex;
  
  /// Value will be returned by @ref flup_iterator.hasNext implementation
  bool hasNext;
  /// Pointer to next value
  void* nextValue;
} flup_ilist_iterator;

/**
 * @brief Initializes an @ref flup_ilist_iterator and reset it
 * @public @memberof flup_ilist_iterator
 *
 * @param self The instance
 * @param owner The list to be iterated by @p self instance
 *
 * @return Returns result of @ref flup_resetable_iterator.reset
 */
FLUP_PUBLIC
int flup_ilist_iterator_init(flup_ilist_iterator* self, flup_ilist* owner);

#endif

