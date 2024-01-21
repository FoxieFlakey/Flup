
#include "flup/interface/ilist.h"

int flup_impl_ilist_append(flup_ilist* self, const void* element) {
  return self->ops->insert(self, self->ops->length(self), element);
}

int flup_impl_ilist_prepend(flup_ilist* self, const void* element) {
  return self->ops->insert(self, 0, element);
}

