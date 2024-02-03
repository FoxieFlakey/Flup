#include <stddef.h>

#include "flup/data_structs/dyn_array.h"
#include "flup/interface/ilist.h"
#include "flup/container_of.h"

#define getSelf(x) container_of(x, flup_dyn_array, interface.IList)

static void dealloc(flup_ilist* _self) {
  flup_dyn_array_free(getSelf(_self));
}

static size_t length(flup_ilist* _self) {
  return getSelf(_self)->length;
}

static int get(flup_ilist* _self, size_t index, void** element) {
  return flup_dyn_array_get(getSelf(_self), index, element);
}

static int insert(flup_ilist* _self, size_t index, const void* element) {
  return flup_dyn_array_insert(getSelf(_self), index, element);
}

static int removeImpl(flup_ilist* _self, size_t index, size_t count) {
  return flup_dyn_array_remove(getSelf(_self), index, count);
}

static size_t elementSize(flup_ilist* _self) {
  return getSelf(_self)->elementSize;
}

const flup_ilist_ops dyn_array_ilist_ops = {
  FLUP_ILIST_OPS_DEFAULTS
  .length = length,
  .elementSize = elementSize,
  .insert = insert,
  .remove = removeImpl,
  .get = get,
  .dealloc = dealloc
};

