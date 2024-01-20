#ifndef header_1705728056_a27e30b2_3cba_46ed_8603_f86729fdf4ca_dyn_array_h
#define header_1705728056_a27e30b2_3cba_46ed_8603_f86729fdf4ca_dyn_array_h

// Dynamicly expanding array

#include <stddef.h>

#include "flup/attributes.h"
#include "flup/tags.h"

typedef struct flup_dyn_array {
  unsigned int length;
  unsigned int capacity;
  size_t elementSize;
  void* array;
} flup_dyn_array;

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_dyn_array* flup_dyn_array_new(size_t elementSize, unsigned int capacity);

FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_reserve(flup_dyn_array* self, unsigned int count);

FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_dyn_array_free(flup_dyn_array* self);

FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_append(flup_dyn_array* self, void* element);

FLUP_PUBLIC
void* flup_dyn_array_at(flup_dyn_array* self, unsigned int index);

#endif
