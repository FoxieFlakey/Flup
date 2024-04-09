#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "flup/data_structs/buffer/circular_buffer.h"
#include "flup/attributes.h"

// Almost copy pasting from https://lo.calho.st/posts/black-magic-buffer/

FLUP_PUBLIC
flup_circular_buffer* flup_circular_buffer_new(size_t size) {
  flup_circular_buffer* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_circular_buffer) {
    .bufferSize = size
  };
  
  void* buffer = malloc(size);
  if (!buffer) {
    flup_circular_buffer_free(self);
    return NULL;
  }
  self->buffer = buffer;
  return self;
}

FLUP_PUBLIC
void flup_circular_buffer_free(flup_circular_buffer* self) {
  if (!self)
    return;
  free(self->buffer);
  free(self);
}

FLUP_PUBLIC
int flup_circular_buffer_write(flup_circular_buffer* self, const void* data, size_t size) {
  // Buffer is full
  if (size > self->bufferSize - self->usedCount)
    return -ENOSPC;
  
  const char* charData = data;
  char* charBuffer = self->buffer;
  for (size_t i = 0; i < size; i++)
    charBuffer[(self->writeOffset + i) % self->bufferSize] = charData[i];
  
  self->writeOffset = (self->writeOffset + size) % self->bufferSize;
  self->usedCount += size;
  return 0;
}

FLUP_PUBLIC
int flup_circular_buffer_read(flup_circular_buffer* self, void* data, size_t size) {
  // Buffer is empty
  if (flup_circular_buffer_is_empty(self))
    return -ENODATA;
  
  char* charData = data;
  const char* charBuffer = self->buffer;
  for (size_t i = 0; i < size; i++)
    charData[i] = charBuffer[(self->readOffset + i) % self->bufferSize];
  
  self->readOffset = (self->readOffset + size) % self->bufferSize;
  self->usedCount -= size;
  return 0;
}

FLUP_PUBLIC
bool flup_circular_buffer_is_empty(flup_circular_buffer* self) {
  return self->usedCount == 0;
}

FLUP_PUBLIC
bool flup_circular_buffer_is_full(flup_circular_buffer* self) {
  return self->usedCount == self->bufferSize;
}


