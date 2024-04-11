#ifndef header_1703918869_21e715cb_968d_4b3b_a989_515a06361e97_iterator_h
#define header_1703918869_21e715cb_968d_4b3b_a989_515a06361e97_iterator_h

#include <stddef.h>

#include "flup/attributes.h"
#include "flup/typecheck.h"

/**
 * @file
 * @brief Macros and type for stateful iterator
 */

typedef struct flup_iterator flup_iterator;

/**
 * @brief Contains default @ref flup_iterator initializers
 *
 * This should be initialize the @ref flup_iterator first like
 *
 * ```c
 * struct flup_iterator_ops = { FLUP_ITERATOR_OPS_DEFAULT
 *   // <app's own implementations>
 * }
 * ```
 *
 * To ensure forward compatibility of the source code
 */
#define FLUP_ITERATOR_OPS_DEFAULT .cleanup = flup_iterator_impl_cleanup_nop,

/**
 * @brief A structure container pointer to various iterator operations
 *
 * This @ref flup_iterator may be allocated on stack therefore
 * this doesn't have `dealloc` function but has `cleanup` instead
 * which cleans and dealloc stuffs but not free the iterator itself
 */
typedef struct flup_iterator_ops {

  /**
   * @brief Advance iterator state
   * @public @memberof flup_iterator
   *
   * Advance current iterator state
   * and optionally copy next value to @p nextItem
   *
   * @note This method may set @ref flup_iterator.errorCode if
   *   any error occured
   *
   * @param self The iterator state
   * @param[out] nextItem The pointer to space to store next item
   *
   * @return False if beyond the end of iteration
   */
  bool (*next)(flup_iterator* self, void* nextItem);

  /**
   * @brief Check if there next item
   * @public @memberof flup_iterator
   *
   * @note This method may set @ref flup_iterator.errorCode if
   *   any error occured
   *
   * @param self The iterator state
   * @return True if there is next item
   */
  bool (*hasNext)(flup_iterator* self);

  /**
   * @brief Do any cleanup necessary except free'ing self
   * @public @memberof flup_iterator
   *
   * @param self The iterator state
   */
  void (*cleanup)(flup_iterator* self);
} flup_iterator_ops;

/**
 * @brief The default iterator initialzer
 *
 * Use this to initialize as part of structure
 * initializer of the @ref flup_iterator
 */
#define FLUP_ITERATOR_DEFAULTS .free = flup_iterator_impl_free_nop, .errorCode = 0, .current = NULL, 

/**
 * @brief Iterator state
 */
typedef struct flup_iterator {
  /// Pointer to operation supported by this iterator
  const flup_iterator_ops* ops;
  
  /**
   * @public @brief Free's current iterator
   *
   * Moving the free function away from @ref flup_iterator.ops
   * allows the caller to possible static allocate or dynamicly
   * allocate the iterator assuming they know the size of the
   * derived iterator and initialize it.
   *
   * @param self The current iterator
   */
  void (*free)(flup_iterator* self); 
  
  /// @public Pointer to current item
  void* current;

  /**
   * @public @brief Recent error code
   *
   * Used to store error codes from iterator methods if an
   * error occured. None of the methods allowed to set this
   * to 0.
   *
   * @warning Method is not mandated to set errno on error.
   *   Set this to 0 before calling an method and check whether
   *   this changed or not.
   */
  int errorCode;
} flup_iterator;

/**
 * @brief Default implementation of @ref flup_iterator.free
 * @protected @memberof flup_iterator
 *
 * This defaults to no-op.
 *
 * @param self The iterator state
 */
FLUP_PUBLIC
void flup_iterator_impl_free_nop(flup_iterator* self);

/**
 * @brief Default implementation of @ref flup_iterator.cleanup
 * @protected @memberof flup_iterator
 *
 * This defaults to no-op.
 *
 * @param self The iterator state
 */
FLUP_PUBLIC
void flup_iterator_impl_cleanup_nop(flup_iterator* self);

/**
 * @brief Macro for iterating each item by value
 *
 * @note This will dereference the item pointer
 * @warning During iteration caller must not modify the structure which
 *   the iterator created from. But caller can modify the item
 *
 * @param itemType Type of item which its iterating
 * @param itemName Identifier of an item
 * @param iterator The @ref flup_iterator state as variable reference
 */
#define flup_iterator_foreach(itemType, itemName, iterator) \
  for (itemType itemName, *const __flup_iterator_state_ ## itemName = (void*) FLUP_CHECK_VAR_TYPE(iterator, flup_iterator*); \
      iterator->ops->next((void*) __flup_iterator_state_ ## itemName , &itemName);)

/**
 * @brief Macro for iterating each item by pointer
 *
 * @note This won't dereference the item pointer
 * @warning During iteration caller must not modify the structure which
 *   the iterator created from. But caller can modify the item
 * 
 * @param itemType Type of item which its iterating
 * @param itemName Identifier of an item
 * @param iterator The @ref flup_iterator state as variable reference
 */
#define flup_iterator_foreach_ptr(itemType, itemName, iterator) \
  for (itemType* itemName, *const __flup_iterator_state_ ## itemName = (void*) FLUP_CHECK_VAR_TYPE(iterator, flup_iterator*); \
      iterator->ops->next((void*) __flup_iterator_state_ ## itemName, NULL) && ((itemName = ((flup_iterator*) __flup_iterator_state_ ## itemName)->current) || true);)

#endif




