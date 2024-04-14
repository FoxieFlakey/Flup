#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "flup/bug.h"
#include "flup/attributes.h"
#include "flup/concurrency/mutex.h"
#include "flup/concurrency/cond.h"

#include "flup/core/panic.h"
#include "flup/data_structs/buffer/circular_buffer.h"
#include "flup/data_structs/buffer.h"

FLUP_PUBLIC
flup_buffer* flup_buffer_new(size_t capacityInBytes) {
  flup_buffer* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_buffer) {};
  if ((self->lock = flup_mutex_new()) == NULL)
    goto failure;
  if ((self->dataWrittenEvent = flup_cond_new()) == NULL)
    goto failure;
  if ((self->dataReadEvent = flup_cond_new()) == NULL)
    goto failure;
  if ((self->buffer = flup_circular_buffer_new(capacityInBytes)) == NULL)
    goto failure;
  return self;

failure:
  flup_buffer_free(self);
  return NULL;
}

FLUP_PUBLIC
void flup_buffer_free(flup_buffer* self) {
  if (!self)
    return;
  
  flup_cond_free(self->dataWrittenEvent);
  flup_cond_free(self->dataReadEvent);
  flup_mutex_free(self->lock);
  flup_circular_buffer_free(self->buffer);
  free(self);
}

FLUP_PUBLIC
int flup_buffer_write(flup_buffer* self, const void* data, size_t size) {
  if (size > self->buffer->bufferSize)
    return -EMSGSIZE;
  
  flup_mutex_lock(self->lock);
  // No space available wait for data read event and then check space
  while (self->buffer->bufferSize - self->buffer->usedSize < size)
    flup_cond_wait(self->dataReadEvent, self->lock, NULL);
  
  int ret = flup_circular_buffer_write(self->buffer, data, size);
  BUG_ON(ret != 0);
  flup_mutex_unlock(self->lock);
  
  flup_cond_wake_all(self->dataWrittenEvent);
  return 0;
}

FLUP_PUBLIC
int flup_buffer_read(flup_buffer* self, void* dataRead, size_t size) {
  if (size > self->buffer->bufferSize)
    return -EMSGSIZE;
  
  flup_mutex_lock(self->lock);
  // Not enough data wait for data written event
  while (self->buffer->usedSize < size)
    flup_cond_wait(self->dataWrittenEvent, self->lock, NULL);
  
  int ret = flup_circular_buffer_read(self->buffer, dataRead, size);
  BUG_ON(ret != 0);
  flup_mutex_unlock(self->lock);
  
  flup_cond_wake_all(self->dataWrittenEvent);
  return 0;
}

FLUP_PUBLIC
void flup_buffer_flush(flup_buffer* self) {
  flup_mutex_lock(self->lock);
  while (self->buffer->usedSize > 0)
    flup_cond_wait(self->dataReadEvent, self->lock, NULL);
  flup_mutex_unlock(self->lock);
}

FLUP_PUBLIC
void flup_buffer_write_no_fail(flup_buffer* self, const void* data, size_t size) {
  int ret = flup_buffer_write(self, data, size);
  if (ret != 0)
    flup_panic("flup_buffer_write failed in no fail context, error %d", ret);
}

FLUP_PUBLIC
void flup_buffer_read_no_fail(flup_buffer* self, void* readData, size_t size) {
  int ret = flup_buffer_write(self, readData, size);
  if (ret != 0)
    flup_panic("flup_buffer_read failed in no fail context, error %d", ret);
}


