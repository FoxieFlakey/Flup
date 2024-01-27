#include <stdckdint.h>
#include <stddef.h>
#include <errno.h>
#include <stdio.h>

#include "flup/bug.h"
#include "flup/data_structs/linked_list.h"
#include "flup/data_structs/list_head.h"
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

static int get(flup_ilist* self, size_t index, void** element) {
  if (index >= getSelf(self)->length)
    return false;
  
  flup_linked_list_node* node = flup_linked_list_at_index(getSelf(self), index);
  if (element)
    *element = node->data;
  return true;
}

static size_t elementSize(flup_ilist* self) {
  return getSelf(self)->elementSize;
}

static size_t length(flup_ilist* self) {
  return getSelf(self)->length;
}

static int removeImpl(flup_ilist* _self, size_t index, size_t count) {
  flup_linked_list* self = getSelf(_self);
  if (count == 0 || index >= self->length)
    return -EINVAL;
  size_t lastIndex;
  if (ckd_add(&lastIndex, index, count))
    return -EINVAL;
  if (lastIndex >= self->length)
    return -EINVAL;

  flup_linked_list_node* node = flup_linked_list_at_index(self, index);
  for (size_t i = 0; i < count; i++) {
    flup_linked_list_node* next = flup_list_entry(node->node.next, flup_linked_list_node, node);
    
    // Do deletion
    BUG_ON(node == NULL || flup_list_is_head(&self->list, &node->node));
    flup_linked_list_del(self, node);
    
    node = next;
  }
  return 0;
}

const flup_ilist_ops linked_list_ilist_ops = {
  FLUP_ILIST_OPS_DEFAULTS
  .dealloc = dealloc,
  .append = append,
  .prepend = prepend,
  .get = get,
  .elementSize = elementSize,
  .length = length,
  .remove = removeImpl
};

