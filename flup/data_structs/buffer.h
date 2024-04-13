#ifndef header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h
#define header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h

#include <stddef.h>

#include "flup/concurrency/cond.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/buffer/circular_buffer.h"

typedef struct flup_buffer {
  flup_mutex* lock;
  flup_cond* dataWrittenEvent;
  flup_cond* dataReadEvent;
  
  flup_circular_buffer* buffer;
} flup_buffer;

flup_buffer* flup_buffer_new(size_t capacityInBytes);
void flup_buffer_free(flup_buffer* self);

int flup_buffer_write(flup_buffer* self, const void* data, size_t size);
int flup_buffer_read(flup_buffer* self, void* readData, size_t size);

// Wait until buffer is completely empty
void flup_buffer_flush(flup_buffer* self);

#endif

