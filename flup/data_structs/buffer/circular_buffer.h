#ifndef UWU_D97A6D5D_56B3_4FDF_8623_0E075A3B7D06_UWU
#define UWU_D97A6D5D_56B3_4FDF_8623_0E075A3B7D06_UWU

#include <stddef.h>
#include <stdckdint.h>

typedef struct circular_buffer {
  ckd_size_t headOffset;
  ckd_size_t tailOffset;
  
  size_t bufferSize;
  void* buffer;
} circular_buffer;

#endif
