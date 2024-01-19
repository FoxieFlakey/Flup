#include <stddef.h>
#include <stdlib.h>

#include "flup/bug.h"
#include "flup/container_of.h"
#include "linked_list_types.h"

#include "flup/util/iterator.h"
#include "flup/attributes.h"
#include "flup/data_structs/list_head.h"
#include "flup/data_structs/linked_list.h"

FLUP_PUBLIC
flup_linked_list* flup_linked_list_new() {
  flup_linked_list* self = malloc(sizeof(*self));
  if (!self)
    return NULL;

  *self = (flup_linked_list) {
    .count = 0,
    .list = FLUP_LIST_HEAD_INIT(self->list)
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
    flup_linked_list_del(self, flup_list_entry(current, flup_linked_node, node));
  free(self);
}

static bool nextItem(flup_iterator_state* _state) {
  struct linked_list_iterator* state = container_of(_state, struct linked_list_iterator, state);
  flup_linked_list* self = state->owner;
  flup_linked_node* cur = state->next;
  BUG_ON(state->knownVersion != self->version);

  // Reached head, iteration ended
  if (flup_list_is_head(&cur->node, &self->list))
    return false;
 
  state->state.current = cur->data;
  state->next = flup_list_entry(cur->node.next, flup_linked_node, node);
  return true;
}

static bool hasNext(flup_iterator_state* _state) {
  struct linked_list_iterator* state = container_of(_state, struct linked_list_iterator, state);
  return !flup_list_is_head(&state->next->node, &state->owner->list);
}

static void freeIterator(flup_iterator_state* _state) {
  struct linked_list_iterator* state = container_of(_state, struct linked_list_iterator, state);
  free(state);
}

static int resetIterator(flup_iterator_state* _state) {
  struct linked_list_iterator* state = container_of(_state, struct linked_list_iterator, state);
  state->state.current = NULL;
  state->state.errorCode = 0;
  state->next = flup_list_first_entry(&state->owner->list, flup_linked_node, node);
  return 0;
}

FLUP_PUBLIC
flup_iterator_state* flup_linked_list_iterator(flup_linked_list* self) {
  struct linked_list_iterator* iterator = malloc(sizeof(*iterator));
  if (!iterator)
    return NULL;

  static flup_iterator_ops ops = { FLUP_ITERATOR_OPS_DEFAULT
    .next = nextItem,
    .hasNext = hasNext,
    .free = freeIterator,
    .reset = resetIterator
  };

  *iterator = (struct linked_list_iterator) {
    .next = flup_list_first_entry(&self->list, flup_linked_node, node),
    .owner = self,
    .state = {
      .current = NULL,
      .errorCode = 0,
      .ops = &ops
    }
  };
  return &iterator->state;
}

FLUP_PUBLIC
void flup_linked_list_del(flup_linked_list*, flup_linked_node* node) {
  flup_list_del(&node->node);
  free(node);
}

FLUP_PUBLIC
flup_linked_node* flup_linked_list_add_tail(flup_linked_list* self, void* data) {
  flup_linked_node* node = malloc(sizeof(*node));
  if (!node)
    return NULL;

  node->data = data;
  flup_list_add_tail(&self->list, &node->node);
  return node;
}

FLUP_PUBLIC
flup_linked_node* flup_linked_list_add_head(flup_linked_list* self, void* data) {
  flup_linked_node* node = malloc(sizeof(*node));
  if (!node)
    return NULL;

  node->data = data;
  flup_list_add_head(&self->list, &node->node);
  return node;
}




