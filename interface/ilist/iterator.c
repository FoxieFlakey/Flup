#include <stddef.h>
#include <string.h>

#include "flup/util/iterator.h"
#include "flup/container_of.h"
#include "flup/interface/ilist.h"

#include "interface/ilist/iterator.h"

#define getSelf(x) container_of((x), struct ilist_iterator, super)

static bool hasNext(flup_iterator* self) {
  return getSelf(self)->hasNext;
}

static bool next(flup_iterator* _self, void* current) {
  struct ilist_iterator* self = getSelf(_self);
  if (!self->hasNext)
    return false;
  
  // Advance the current and get next value
  _self->current = self->nextValue;
  self->nextIndex++;
  
  int ret = self->owner->ops->get(self->owner, self->nextIndex, &self->nextValue);
  if (ret < 0) {
    self->super.errorCode = -ret;
    return false;
  }
  self->hasNext = ret != 0;

  if (current)
    memcpy(current, _self->current, self->owner->ops->elementSize(self->owner));
  return true;
}

static int reset(flup_iterator* _self) {
  struct ilist_iterator* self = getSelf(_self);
  self->nextIndex = 0;
  self->nextValue = NULL;
  self->hasNext = false;

  int ret = self->owner->ops->get(self->owner, 0, &self->nextValue);
  if (ret < 0)
    return ret;
  self->hasNext = !!ret;
  return 0;
}

const flup_iterator_ops IList_iterator_ops = {
  FLUP_ITERATOR_OPS_DEFAULT
  .hasNext = hasNext,
  .next = next,
  .reset = reset
};

int ilist_iterator_init(struct ilist_iterator* self, flup_ilist* owner) {
  self->owner = owner;
  self->super = (flup_iterator) {
    FLUP_ITERATOR_DEFAULTS
    .ops = &IList_iterator_ops,
  };
  return self->super.ops->reset(&self->super);
}



