#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "flup/bug.h"
#include "flup/container_of.h"
#include "flup/util/iterator.h"
#include "flup/attributes.h"
#include "flup/data_structs/list_head.h"
#include "flup/data_structs/linked_list.h"

#include "data_structs/linked_list_impl_ilist.h"
#include "flup/util/iterator_resetable.h"

#define getSelf(x) container_of(x, flup_linked_list_iterator, super.super)
#define getSelfFromResetable(x) container_of(x, flup_linked_list_iterator, super)

FLUP_PUBLIC
flup_linked_list* flup_linked_list_new(size_t elementSize) {
  flup_linked_list* self = malloc(sizeof(*self));
  if (!self)
    return NULL;

  *self = (flup_linked_list) {
    .list = FLUP_LIST_HEAD_INIT(self->list),
    .length = 0,
    .elementSize = elementSize,
    .interface = {
      .IList.ops = &linked_list_ilist_ops
    }
  };
  return self;
}

FLUP_PUBLIC
void flup_linked_list_free(flup_linked_list* self) {
  if (!self)
    return;

  flup_list_head* current;
  flup_list_head* next;
  flup_list_for_each_safe(&self->list, current, next)
    flup_linked_list_del(self, flup_list_entry(current, flup_linked_list_node, node));
  free(self);
}

static bool nextItem(flup_iterator* _state, void* current) {
  flup_linked_list_iterator* state = getSelf(_state);
  flup_linked_list* self = state->owner;
  flup_linked_list_node* cur = state->next;
  BUG_ON(state->knownVersion != self->version);

  // Reached head, iteration ended
  if (flup_list_is_head(&cur->node, &self->list)) {
    state->super.super.errorCode = 0;
    return false;
  }
 
  state->super.super.current = &cur->data;
  state->next = flup_list_entry(cur->node.next, flup_linked_list_node, node);

  // Copies current value if requested
  if (current)
    memcpy(current, &cur->data, self->elementSize);
  return true;
}

static bool hasNext(flup_iterator* _state) {
  flup_linked_list_iterator* state = getSelf(_state);
  return !flup_list_is_head(&state->next->node, &state->owner->list);
}

static int resetIterator(flup_resetable_iterator* _state) {
  flup_linked_list_iterator* state = getSelfFromResetable(_state);
  state->super.super.current = NULL;
  state->super.super.errorCode = 0;
  state->next = flup_list_first_entry(&state->owner->list, flup_linked_list_node, node);
  return 0;
}

FLUP_PUBLIC
void flup_linked_list_get_iterator(flup_linked_list* self, flup_linked_list_iterator* preAllocated) {
  static flup_iterator_ops ops = { FLUP_ITERATOR_OPS_DEFAULT
    .next = nextItem,
    .hasNext = hasNext
  };
  
  static flup_resetable_iterator_ops resetableOps = { FLUP_RESETABLE_ITERATOR_DEFAULTS_OPS
    .reset = resetIterator
  };

  *preAllocated = (flup_linked_list_iterator) {
    .next = flup_list_first_entry(&self->list, flup_linked_list_node, node),
    .owner = self,
    .super = {
      .ops = &resetableOps,
      .super = (flup_iterator) { FLUP_ITERATOR_DEFAULTS
        .current = NULL,
        .errorCode = 0,
        .ops = &ops
      }
    }
  };
}

FLUP_PUBLIC
void flup_linked_list_del(flup_linked_list* self, flup_linked_list_node* node) {
  self->length--;
  flup_list_del(&node->node);
  free(node);
}

FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_add_tail(flup_linked_list* self, const void* data) {
  flup_linked_list_node* node = malloc(sizeof(*node) + self->elementSize);
  if (!node)
    return NULL;

  memcpy(node->data, data, self->elementSize);
  
  flup_list_add_tail(&self->list, &node->node);
  self->length++;
  return node;
}

FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_add_head(flup_linked_list* self, const void* data) {
  flup_linked_list_node* node = malloc(sizeof(*node) + self->elementSize);
  if (!node)
    return NULL;

  memcpy(node->data, data, self->elementSize);
  
  flup_list_add_head(&self->list, &node->node);
  self->length++;
  return node;
}

FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_at_index(flup_linked_list* self, size_t index) {
  if (index >= self->length)
    return NULL;

  flup_list_head* current;
  size_t i = 0;
  flup_list_for_each(&self->list, current) {
    if (i == index)
      return flup_list_entry(current, flup_linked_list_node, node);
    i++;
  }
  return NULL;
}



