#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "flup/data_structs/squeue.h"
#include "flup/attributes.h"
#include "flup/container_of.h"
#include "flup/data_structs/list_head.h"
#include "flup/tags.h"

#include "data_structs/squeue_types.h"

FLUP_PUBLIC
flup_squeue_item* flup_squeue_dequeue_filtered_func(flup_squeue* self, flup_squeue_filter_func filter, void* udata) {
  return flup_squeue_dequeue_filtered_blocks(self, ^bool (flup_squeue_item *msg) {
    return filter(msg, udata);
  });
}

static flup_squeue_item* peek(flup_squeue* _self) {
  struct squeue_impl* self = getSqueueImpl(_self);
  flup_list_head* current = flup_list_last(&self->queue);
  if (flup_list_is_head(&self->queue, current))
    return NULL;
  return container_of(current, flup_squeue_item, node);
}

static void removeQueueItem(flup_squeue* _self, flup_squeue_item* item) {
  flup_list_del(&item->node);
  _self->length--;
}

FLUP_PUBLIC
flup_squeue_item* flup_squeue_dequeue_filtered_blocks(flup_squeue* _self, flup_squeue_filter_block filter) {
  flup_squeue_item* current = peek(_self);
  if (!current || !filter(current))
    return NULL;
  removeQueueItem(_self, current);
  return current;
}

FLUP_PUBLIC
flup_squeue_item* flup_squeue_dequeue(flup_squeue* _self) {
  flup_squeue_item* current = peek(_self);
  if (!current)
    return NULL;
  removeQueueItem(_self, current);
  return current;
}

FLUP_PUBLIC
int flup_squeue_enqueue(flup_squeue* _self, flup_squeue_item* msg) {
  struct squeue_impl* self = getSqueueImpl(_self);
  if (_self->length + 1 > _self->capacity)
    return -EOVERFLOW;
  _self->length++;
  flup_list_add_head(&self->queue, &msg->node);
  return 0;
}

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_squeue* flup_squeue_new(unsigned int capacity, unsigned int flags) {
  (void) flags;

  struct squeue_impl* self = malloc(sizeof(*self));
  *self = (struct squeue_impl) {
    .super = {
      .capacity = capacity,
      .length = 0
    }
  };

  flup_list_head_init(&self->queue);
  return &self->super;
}

FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_squeue_free(flup_squeue* _self) {
  struct squeue_impl* self = getSqueueImpl(_self);
  flup_list_head* current;
  flup_list_head* next;
  flup_list_for_each_safe(&self->queue, current, next) {
    flup_squeue_item* msg = flup_list_entry(current, flup_squeue_item, node);
    msg->dealloc(msg);
  }

  free(self);
}

