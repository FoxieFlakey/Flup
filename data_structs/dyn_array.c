#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbit.h>
#include <stdckdint.h>
#include <string.h>

#include "flup/interface/ilist.h"
#include "flup/data_structs/dyn_array.h"
#include "flup/attributes.h"
#include "flup/tags.h"

#include "data_structs/dyn_array_impl_ilist.h"

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_dyn_array* flup_dyn_array_new(size_t elementSize, size_t capacity) {
  flup_dyn_array* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_dyn_array) {
    .capacity = 0,
    .length = 0,
    .elementSize = elementSize
  };

  self->interface.IList = (flup_ilist) {
    .ops = &dyn_array_ilist_ops
  };

  if (flup_dyn_array_reserve(self, capacity) < 0)
    goto failure;
  memset(self->array, 0xBE, self->capacity * self->elementSize);
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
int flup_dyn_array_reserve(flup_dyn_array* self, size_t count) {
  size_t maxLengthReserved;
  if (ckd_add(&maxLengthReserved, self->length, count))
    return -EINVAL;
  
  size_t newCapacity = stdc_bit_ceil(maxLengthReserved);
  if (newCapacity <= self->capacity)
    return 0;

  self->capacity = newCapacity;
  void* newArray = realloc(self->array, self->elementSize * newCapacity);
  if (!newArray)
    return -ENOMEM;
  self->array = newArray;
  return 0;
}

static void* getElementAddr(flup_dyn_array* self, size_t index) {
  return (char*) self->array + self->elementSize * index;
}

static int checkSpaceAndIncrementLength(flup_dyn_array* self, size_t n) {
  size_t newLength;
  if (ckd_add(&newLength, self->length, n))
    return -EOVERFLOW;

  int res = 0;
  if ((res = flup_dyn_array_reserve(self, n)))
    return res;

  self->length = newLength;
  return 0;
}

FLUP_PUBLIC
int flup_dyn_array_insert(flup_dyn_array* self, size_t index, const void* element) {
  size_t moveTarget;
  size_t moveCount;
  if (ckd_add(&moveTarget, index, 1))
    return -EINVAL;
  if (ckd_sub(&moveCount, self->length, index))
    return -EINVAL;
  
  // > instead >= allows index of self->length to
  // append at the end of array
  if (index > self->length)
    return -EINVAL;

  int res = 0;
  if ((res = checkSpaceAndIncrementLength(self, 1)) < 0)
    return res;
  
  if (moveCount > 0)
    memmove(getElementAddr(self, moveTarget), getElementAddr(self, index), moveCount * self->elementSize);
  flup_dyn_array_set(self, index, element);
  return 0;
}

FLUP_PUBLIC
int flup_dyn_array_remove(flup_dyn_array* self, size_t index, size_t count) {
  if (count < 1 || index >= self->length)
    return -EINVAL;
  
  size_t startOfNext;
  if (ckd_add(&startOfNext, index, count))
    return -EINVAL;
  if (startOfNext >= self->length)
    return -EINVAL;

  void* destinationAddr = getElementAddr(self, index);
  void* sourceAddr = getElementAddr(self, startOfNext);
  size_t moveCount = self->length - startOfNext;
  memmove(destinationAddr, sourceAddr, moveCount * self->elementSize);
  self->length -= count;
  return 0;
}

FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_append(flup_dyn_array* self, const void* element) {
  return flup_dyn_array_insert(self, self->length, element);
}

FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_prepend(flup_dyn_array* self, const void* element) {
  return flup_dyn_array_insert(self, 0, element);
}

FLUP_PUBLIC
int flup_dyn_array_get(flup_dyn_array* self, size_t index, void** element) {
  if (index >= self->length)
    return false;

  if (element)
    *element = getElementAddr(self, index);
  return true;
}

FLUP_PUBLIC
int flup_dyn_array_set(flup_dyn_array* self, size_t index, const void* element) {
  if (index >= self->length)
    return -EINVAL;
  
  memcpy(getElementAddr(self, index), element, self->elementSize);
  return 0;
}

FLUP_DEALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_trim(flup_dyn_array* self) {
  size_t trimmedCapacity = stdc_bit_ceil(self->length);
  if (trimmedCapacity == self->capacity)
    return 0;
  
  void* trimmedArray = realloc(self->array, trimmedCapacity * self->elementSize);
  if (!trimmedArray)
    return -ENOMEM;
  self->array = trimmedArray;
  self->capacity = trimmedCapacity;
  return 0;
}

