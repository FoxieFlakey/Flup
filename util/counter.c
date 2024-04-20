#include <stdint.h>
#include <stdatomic.h>
#include <inttypes.h>

#include "flup/util/counter.h"
#include "flup/attributes.h"
#include "flup/core/panic.h"

FLUP_PUBLIC
uint64_t flup_counter_get(flup_counter* self) {
  uint64_t n = atomic_fetch_add(&self->counter, 1);
  if (n == UINT64_MAX)
    flup_panic("Counter at %p reached maximum number which is " PRIu64, self, UINT64_MAX);
  return n;
}

