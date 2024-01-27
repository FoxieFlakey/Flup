#include "flup/util/iterator.h"
#include "flup/attributes.h"

FLUP_PUBLIC
void flup_iterator_impl_free_nop(flup_iterator* self) {
  self->ops->cleanup(self);
}

FLUP_PUBLIC
void flup_iterator_impl_cleanup_nop(flup_iterator*) {
}

