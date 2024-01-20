#ifndef header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h
#define header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h

#include <time.h>

#include "flup/attributes.h"

// Queue interface

typedef struct flup_queue {
  struct flup_queue_ops* ops;
} flup_queue;

typedef struct flup_queue_ops {
  unsigned int (*getLength)(flup_queue* self);
  
  unsigned int (*try_enqueue)(flup_queue* self, void* ptr);
  int (*enqueue)(flup_queue* self, void* ptr);
  
  void* FLUP_NULLABLE (*try_dequeue)(flup_queue* self);
  void* FLUP_NONNULL (*dequeue)(flup_queue* self, const struct timespec* abstimeout);
  
  void (*dealloc)(flup_queue* self);
} flup_queue_ops;

#endif

