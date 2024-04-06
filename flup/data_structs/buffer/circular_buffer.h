#ifndef UWU_D97A6D5D_56B3_4FDF_8623_0E075A3B7D06_UWU
#define UWU_D97A6D5D_56B3_4FDF_8623_0E075A3B7D06_UWU

#include <stddef.h>

#include "flup/attributes.h"

typedef struct flup_circular_buffer {
  size_t readOffset;
  size_t writeOffset;
  
  size_t bufferSize;
  void* buffer;
} flup_circular_buffer;

FLUP_PUBLIC
flup_circular_buffer* flup_circular_buffer_new(size_t size);

FLUP_PUBLIC
void flup_circular_buffer_free(flup_circular_buffer* self);

FLUP_PUBLIC
int flup_circular_buffer_write(flup_circular_buffer* self, const void* data, size_t size);

FLUP_PUBLIC
int flup_circular_buffer_read(flup_circular_buffer* self, void* data, size_t size);

FLUP_PUBLIC
bool flup_circular_buffer_is_empty(flup_circular_buffer* self);

FLUP_PUBLIC
bool flup_circular_buffer_is_full(flup_circular_buffer* self);

#endif
