#include <stddef.h>
#include <string.h>

#include "flup/util/iterator.h"
#include "flup/attributes.h"
#include "flup/container_of.h"
#include "flup/interface/ilist.h"
#include "flup/util/iterator_resetable.h"

#define getSelf(x) container_of((x), struct flup_ilist_iterator, super.super)
#define getSelfFromResetable(x) container_of((x), struct flup_ilist_iterator, super)

static bool hasNext(flup_iterator* self) {
  return getSelf(self)->hasNext;
}

static bool next(flup_iterator* _self, void* current) {
  struct flup_ilist_iterator* self = getSelf(_self);
  if (!self->hasNext)
    return false;
  
  // Advance the current and get next value
  _self->current = self->nextValue;
  self->nextIndex++;
  
  int ret = self->owner->ops->get(self->owner, self->nextIndex, &self->nextValue);
  if (ret < 0) {
    self->super.super.errorCode = -ret;
    return false;
  }
  self->hasNext = (bool) ret;

  if (current)
    memcpy(current, _self->current, self->owner->ops->elementSize(self->owner));
  return true;
}

static int reset(flup_resetable_iterator* _self) {
  struct flup_ilist_iterator* self = getSelfFromResetable(_self);
  self->nextIndex = 0;
  self->nextValue = NULL;
  self->hasNext = false;

  int ret = self->owner->ops->get(self->owner, 0, &self->nextValue);
  if (ret < 0)
    return ret;
  self->hasNext = (bool) ret;
  return 0;
}

static const flup_iterator_ops iteratorOps = {
  FLUP_ITERATOR_OPS_DEFAULT
  .hasNext = hasNext,
  .next = next
};

static const flup_resetable_iterator_ops resetableIteratorOps = {
  FLUP_RESETABLE_ITERATOR_DEFAULTS_OPS
  .reset = reset
};

FLUP_PUBLIC
int flup_ilist_iterator_init(struct flup_ilist_iterator* self, flup_ilist* owner) {
  self->owner = owner;
  self->super = (flup_resetable_iterator) {
    .ops = &resetableIteratorOps,
    .super = (flup_iterator) {
      FLUP_ITERATOR_DEFAULTS
      .ops = &iteratorOps,
    }
  };
  return self->super.ops->reset(&self->super);
}



