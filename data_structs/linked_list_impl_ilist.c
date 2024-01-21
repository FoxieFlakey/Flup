#include <stddef.h>
#include <errno.h>

#include "flup/data_structs/linked_list.h"
#include "flup/interface/ilist.h"
#include "flup/container_of.h"

#define getSelf(x) container_of(x, flup_linked_list, interface.IList)

static void dealloc(flup_ilist* self) {
  flup_linked_list_free(getSelf(self));
}

static int append(flup_ilist* self, const void* element) {
  if (flup_linked_list_add_tail(getSelf(self), element) == NULL)
    return -ENOMEM;
  return 0;
}

static int prepend(flup_ilist* self, const void* element) {
  if (flup_linked_list_add_head(getSelf(self), element) == NULL)
    return -ENOMEM;
  return 0;
}

const flup_ilist_ops linked_list_ilist_ops = {
  FLUP_ILIST_OPS_DEFAULTS
  .dealloc = dealloc,
  .append = append,
  .prepend = prepend
};

