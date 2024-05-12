#ifndef UWU_EE5416D8_E140_4854_81A2_C0EC1608128B_UWU
#define UWU_EE5416D8_E140_4854_81A2_C0EC1608128B_UWU

#include <stdatomic.h>

#include "flup/attributes.h"

typedef struct flup_refcount {
  atomic_uint_fast64_t usageCount;
} flup_refcount;

FLUP_PUBLIC
void flup_refcount_init(flup_refcount* self);

FLUP_PUBLIC
void flup_refcount_acquire(flup_refcount* self);

FLUP_PUBLIC
bool flup_refcount_release(flup_refcount* self);

#endif
