#include <stdlib.h>

#include "flup/interface/ilist.h"
#include "flup/attributes.h"
#include "flup/container_of.h"
#include "flup/util/iterator.h"

#include "interface/ilist/iterator.h"

FLUP_PUBLIC
int flup_impl_ilist_append(flup_ilist* self, const void* element) {
  return self->ops->insert(self, self->ops->length(self), element);
}

FLUP_PUBLIC
int flup_impl_ilist_prepend(flup_ilist* self, const void* element) {
  return self->ops->insert(self, 0, element);
}

static void doFreeIterator(flup_iterator* self) {
  free(container_of(self, struct ilist_iterator, super));
}

FLUP_PUBLIC
flup_iterator* flup_impl_ilist_get_iterator(flup_ilist* self) {
  struct ilist_iterator* iterator = malloc(sizeof(*iterator));
  if (ilist_iterator_init(iterator, self) < 0) {
    free(iterator);
    return NULL;
  }
  iterator->super.free = doFreeIterator;
  return &iterator->super;
}

FLUP_PUBLIC
bool flup_impl_ilist_try_get(flup_ilist* self, size_t index, void** element) {
  int ret = self->ops->get(self, index, element);
  
  // Error treated as not exist
  if (ret < 0)
    return false;
  return !!ret;
}



