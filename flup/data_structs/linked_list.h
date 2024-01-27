#ifndef header_1703857689_10eef286_2ea9_4689_a16e_c695ec779995_linked_list_h
#define header_1703857689_10eef286_2ea9_4689_a16e_c695ec779995_linked_list_h

#include <stddef.h>

#include "flup/data_structs/list_head.h"
#include "flup/interface/ilist.h"
#include "flup/util/iterator.h"
#include "flup/tags.h"
#include "flup/attributes.h"

typedef struct flup_linked_list_node {
  flup_list_head node;
  char data[];
} flup_linked_list_node;

typedef struct flup_linked_list {
  struct {
    flup_ilist IList;
  } interface;
  size_t length;
  size_t elementSize;
  
  flup_list_head list;
  int version;
} flup_linked_list;

typedef struct flup_linked_list_iterator {
  flup_iterator super;
  flup_linked_list* owner;
  flup_linked_list_node* next;

  int knownVersion;
} flup_linked_list_iterator;

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_list* flup_linked_list_new(size_t elementSize);

FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_linked_list_free(flup_linked_list* self);

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_add_tail(flup_linked_list* self, const void* data);

FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_add_head(flup_linked_list* self, const void* data);

FLUP_PUBLIC
void flup_linked_list_get_iterator(flup_linked_list* self, flup_linked_list_iterator* preAllocated);

FLUP_PUBLIC
void flup_linked_list_del(flup_linked_list* self, flup_linked_list_node* node);

FLUP_PUBLIC
flup_linked_list_node* flup_linked_list_at_index(flup_linked_list* self, size_t index);

#endif

