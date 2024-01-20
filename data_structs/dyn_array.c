#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbit.h>
#include <stdckdint.h>
#include <string.h>

#include "flup/data_structs/dyn_array.h"
#include "flup/attributes.h"
#include "flup/tags.h"

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_dyn_array* flup_dyn_array_new(size_t elementSize, unsigned int capacity) {
  flup_dyn_array* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_dyn_array) {
    .capacity = 0,
    .length = 0,
    .elementSize = elementSize
  };
  
  if (flup_dyn_array_reserve(self, capacity) < 0)
    goto failure;
  return self;

failure:
  flup_dyn_array_free(self);
    return NULL;
}

FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_dyn_array_free(flup_dyn_array* self) {
  if (!self)
    return;
  
  free(self->array);
  free(self); 
}


FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_reserve(flup_dyn_array* self, unsigned int count) {
  unsigned int maxLengthReserved;
  if (ckd_add(&maxLengthReserved, self->length, count))
    return -EINVAL;
  
  unsigned int newCapacity = stdc_bit_ceil(maxLengthReserved);
  if (newCapacity <= self->capacity)
    return 0;

  self->capacity = newCapacity;
  void* newArray = realloc(self->array, self->elementSize * newCapacity);
  if (!newArray)
    return -ENOMEM;
  self->array = newArray;
  return 0;
}

static void* getElementAddr(flup_dyn_array* self, unsigned int index) {
  return (char*) self->array + self->elementSize * index;
}

FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_append(flup_dyn_array* self, void* element) {
  unsigned int newLength;
  if (ckd_add(&newLength, self->length, 1))
    return -EOVERFLOW;

  int res = 0;
  if ((res = flup_dyn_array_reserve(self, 1)))
    return res;

  memcpy(getElementAddr(self, self->length), element, self->elementSize);
  self->length = newLength;
  return res;
}

FLUP_PUBLIC
void* flup_dyn_array_at(flup_dyn_array* self, unsigned int index) {
  if (index > self->length)
    return NULL;

  return getElementAddr(self, index);
}

