#ifndef header_1703857689_10eef286_2ea9_4689_a16e_c695ec779995_linked_list_h
#define header_1703857689_10eef286_2ea9_4689_a16e_c695ec779995_linked_list_h

#include <stddef.h>

#include "flup/interface/ilist.h"
#include "flup/util/iterator.h"
#include "flup/tags.h"
#include "flup/attributes.h"

typedef struct flup_linked_list {
  struct {
    flup_ilist IList;
  } interface;
  size_t length;
  size_t elementSize;
} flup_linked_list;
typedef struct flup_linked_node flup_linked_node;

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_list* flup_linked_list_new(size_t elementSize);

FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_linked_list_free(flup_linked_list* self);

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_node* flup_linked_list_add_tail(flup_linked_list* self, const void* data);

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_node* flup_linked_list_add_head(flup_linked_list* self, const void* data);

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_iterator_state* flup_linked_list_iterator(flup_linked_list* self);

FLUP_PUBLIC
void flup_linked_list_del(flup_linked_list* self, flup_linked_node* node);

#endif

