#ifndef header_1705058311_67951c3a_bf88_406f_89af_ffe6d430db65_flup_squeue_h
#define header_1705058311_67951c3a_bf88_406f_89af_ffe6d430db65_flup_squeue_h

#include "flup/attributes.h"
#include "flup/compiler.h"
#include "flup/data_structs/list_head.h"
#include "flup/tags.h"

typedef struct flup_squeue {
  unsigned int capacity;
  unsigned int length;
} flup_squeue;

// The memory of this struct managed is
// managed by program
typedef struct flup_squeue_item {
  flup_list_head node;
  void (*dealloc)(struct flup_squeue_item* self);
} flup_squeue_item;

FLUP_USED
static void flup_squeue_nop_msg_dealloc(flup_squeue_item*) {
}

#define FLUP_SQUEUE_ITEM_DEFAULTS .node = {}, .dealloc = flup_squeue_nop_msg_dealloc,

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_squeue* flup_squeue_new(unsigned int capacity, unsigned int flags);

FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_squeue_free(flup_squeue* self);

FLUP_PUBLIC
int flup_squeue_enqueue(flup_squeue* self, flup_squeue_item* msg);

FLUP_PUBLIC
flup_squeue_item* flup_squeue_try_dequeue(flup_squeue* self);

typedef bool (*flup_squeue_filter_func)(flup_squeue_item* msg, void* udata);

FLUP_PUBLIC
flup_squeue_item* flup_squeue_dequeue_filtered_func(flup_squeue* self, flup_squeue_filter_func filter, void* udata);
#define flup_squeue_dequeue_filtered flup_squeue_desqueue_filtered_func

#ifdef FLUP_APPLE_BLOCKS_AVAILABLE
typedef bool (^flup_squeue_filter_block)(flup_squeue_item* msg);

FLUP_PUBLIC
flup_squeue_item* flup_squeue_dequeue_filtered_blocks(flup_squeue* self, flup_squeue_filter_block filter);
# undef flup_squeue_dequeue_filtered
# define flup_squeue_dequeue_filtered flup_squeue_dequeue_filtered_blocks
#endif

#endif





