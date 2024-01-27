#ifndef header_1703918869_21e715cb_968d_4b3b_a989_515a06361e97_iterator_h

#define header_1703918869_21e715cb_968d_4b3b_a989_515a06361e97_iterator_h

#include <stddef.h>

#include "flup/attributes.h"
#include "flup/typecheck.h"

typedef struct flup_iterator flup_iterator;

#define FLUP_ITERATOR_OPS_DEFAULT .cleanup = flup_iterator_impl_cleanup_nop,
typedef struct flup_iterator_ops {

  // Advance current iterator state
  // and optionally copy current value to "current"
  bool (*next)(flup_iterator* self, void* current);

  // Check if current iterator can advance
  bool (*hasNext)(flup_iterator* self);

  // Reset current iterator to start at beginning
  // 0 if success, -errno on error
  int (*reset)(flup_iterator* self);
  
  // Do any cleanup necessary except free'ing self
  void (*cleanup)(flup_iterator* self);
} flup_iterator_ops;

#define FLUP_ITERATOR_DEFAULTS .free = flup_iterator_impl_free_nop, .errorCode = 0, .current = NULL, 
struct flup_iterator {
  const flup_iterator_ops* ops;
  
  // Seperate from "ops" because state may be
  // stack or static or heap allocated
  // Release resources used by current iterator
  void (*free)(flup_iterator* self); 
  
  // Current position in iteration
  void* current;

  // Plain errno
  // 0 if loop quit isn't caused by an error
  int errorCode;
};

FLUP_PUBLIC
void flup_iterator_impl_free_nop(flup_iterator* self);

FLUP_PUBLIC
void flup_iterator_impl_cleanup_nop(flup_iterator*);

#define flup_iterator_foreach(itemType, itemName, iterator) \
  for (itemType itemName, *const __flup_iterator_state = (void*) FLUP_CHECK_VAR_TYPE(iterator, flup_iterator*); \
      iterator->ops->next((void*) __flup_iterator_state, &itemName);)

#define flup_iterator_foreach_ptr(itemType, itemName, iterator) \
  for (itemType* itemName, *const __flup_iterator_state = (void*) FLUP_CHECK_VAR_TYPE(iterator, flup_iterator*); \
      iterator->ops->next((void*) __flup_iterator_state, NULL) && ((itemName = ((flup_iterator*) __flup_iterator_state)->current) || true);)

#endif




