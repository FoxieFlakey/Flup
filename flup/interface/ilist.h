#ifndef header_1705806604_1d522638_a3ed_4c40_bf4c_0ccec5043563_list_h
#define header_1705806604_1d522638_a3ed_4c40_bf4c_0ccec5043563_list_h

#include <stddef.h>

#include "flup/attributes.h"
#include "flup/util/iterator.h"

// Generic list

typedef struct flup_ilist {
  const struct flup_ilist_ops* ops;
} flup_ilist;

typedef struct flup_ilist_ops {
  size_t (*length)(flup_ilist* self);
  size_t (*elementSize)(flup_ilist* self);
  int (*insert)(flup_ilist* self, size_t index, const void* element);
  int (*remove)(flup_ilist* self, size_t index, size_t count);
  
  // Returns 1 if exist and 0 if not exist
  // -errno on error
  int (*get)(flup_ilist* self, size_t index, void** element);
  
  void (*dealloc)(flup_ilist* self);
  
  // Optional with default
  int (*append)(flup_ilist* self, const void* element);
  int (*prepend)(flup_ilist* self, const void* element);
  flup_iterator* (*getIterator)(flup_ilist* self);  
  bool (*tryGet)(flup_ilist* self, size_t index, void** element);
} flup_ilist_ops;

FLUP_PUBLIC
int flup_impl_ilist_append(flup_ilist* self, const void* element);

FLUP_PUBLIC
int flup_impl_ilist_prepend(flup_ilist* self, const void* element);

FLUP_PUBLIC
flup_iterator* flup_impl_ilist_get_iterator(flup_ilist* self);

FLUP_PUBLIC
bool flup_impl_ilist_try_get(flup_ilist* self, size_t index, void** element);

#define FLUP_ILIST_OPS_DEFAULTS \
  .append = flup_impl_ilist_append, \
  .prepend = flup_impl_ilist_prepend, \
  .getIterator = flup_impl_ilist_get_iterator, \
  .tryGet = flup_impl_ilist_try_get,

#endif

