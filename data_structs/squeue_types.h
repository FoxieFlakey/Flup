#ifndef header_1705125294_85f3c3b3_a6f5_44da_a642_506a01ea0972_queue_types_h
#define header_1705125294_85f3c3b3_a6f5_44da_a642_506a01ea0972_queue_types_h

#include "flup/container_of.h"
#include "flup/data_structs/list_head.h"
#include "flup/data_structs/squeue.h"

struct squeue_impl {
  flup_squeue super;
  flup_list_head queue;
};

static inline struct squeue_impl* getSqueueImpl(flup_squeue* self) {
  return container_of(self, struct squeue_impl, super);
}

#endif

