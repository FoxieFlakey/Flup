#include <stdatomic.h>
#include <stdint.h>

#include "flup/attributes.h"
#include "flup/core/panic.h"
#include "flup/util/refcount.h"

FLUP_PUBLIC
void flup_refcount_init(flup_refcount* self) {
  *self = (flup_refcount) {
    .usageCount = 1
  };
}

FLUP_PUBLIC
void flup_refcount_acquire(flup_refcount* self) {
  uint64_t old = atomic_load(&self->usageCount);
  uint64_t new;
  do {
    if (old == UINT64_MAX)
      flup_panic("Ref count for %p exceeded 64-bit refcount limit TwT (possible leak or just someone found the use of uhh 18 quadrallion uses)", self);
    else if (old == 0)
      flup_panic("Ref count for %p is 0 during acquire OwO (use after free is very likely)", self);
    new = old + 1;
  } while (atomic_compare_exchange_weak(&self->usageCount, &old, new));
}

FLUP_PUBLIC
bool flup_refcount_release(flup_refcount* self) {
  uint64_t old = atomic_load(&self->usageCount);
  uint64_t new;
  do {
    if (old == 0)
      flup_panic("Ref count for %p is 0 during release OwO (unbalanced release is very likely)", self);
    new = old - 1;
  } while (atomic_compare_exchange_weak(&self->usageCount, &old, new));
  
  return new == 0;
}

