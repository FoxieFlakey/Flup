#ifndef header_1706342178_8ee8a55b_a60a_4a68_bbe1_1f1a3d9b3bde_iterator_h
#define header_1706342178_8ee8a55b_a60a_4a68_bbe1_1f1a3d9b3bde_iterator_h

#include <stddef.h>

#include "flup/interface/ilist.h"
#include "flup/util/iterator.h"

struct ilist_iterator {
  flup_iterator super;
  flup_ilist* owner;

  size_t nextIndex;
  bool hasNext;
  void* nextValue;
};

int ilist_iterator_init(struct ilist_iterator* self, flup_ilist* owner);

#endif

