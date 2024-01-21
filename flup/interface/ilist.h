#ifndef header_1705806604_1d522638_a3ed_4c40_bf4c_0ccec5043563_list_h
#define header_1705806604_1d522638_a3ed_4c40_bf4c_0ccec5043563_list_h

#include <stddef.h>

// Generic list

typedef struct flup_ilist {
  const struct flup_ilist_ops* ops;
  unsigned int version;
} flup_ilist;

typedef struct flup_ilist_ops {
  size_t (*length)(flup_ilist* self);
  int (*insert)(flup_ilist* self, size_t index, const void* element);
  int (*remove)(flup_ilist* self, size_t index, size_t count);
  int (*get)(flup_ilist* self, size_t index, void** element);
  void (*dealloc)(flup_ilist* self);
  
  // Optional with default
  int (*append)(flup_ilist* self, const void* element);
  int (*prepend)(flup_ilist* self, const void* element);
} flup_ilist_ops;

int flup_impl_ilist_append(flup_ilist* self, const void* element);
int flup_impl_ilist_prepend(flup_ilist* self, const void* element);

#define FLUP_ILIST_OPS_DEFAULTS \
  .append = flup_impl_ilist_append, \
  .prepend = flup_impl_ilist_prepend, 

#endif

