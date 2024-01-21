#ifndef header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h
#define header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h

#include <time.h>

#include "flup/attributes.h"

// Queue interface

typedef struct flup_iqueue {
  struct flup_iqueue_ops* ops;
} flup_iqueue;

typedef struct flup_iqueue_ops {
  unsigned int (*getLength)(flup_iqueue* self);
  
  unsigned int (*try_enqueue)(flup_iqueue* self, void* ptr);
  int (*enqueue)(flup_iqueue* self, void* ptr);
  
  void* FLUP_NULLABLE (*try_dequeue)(flup_iqueue* self);
  void* FLUP_NONNULL (*dequeue)(flup_iqueue* self, const struct timespec* abstimeout);
  
  void (*dealloc)(flup_iqueue* self);
} flup_iqueue_ops;

#endif

