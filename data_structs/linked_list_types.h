#ifndef header_1703857530_9ce0b33f_fa8e_438c_94a3_6798355c436a_array_linked_list_types_h
#define header_1703857530_9ce0b33f_fa8e_438c_94a3_6798355c436a_array_linked_list_types_h

// Type definition for private stuffs

#include "flup/util/iterator.h"
#include "flup/data_structs/linked_list.h"
#include "flup/data_structs/list_head.h"

struct linked_list_impl {
  flup_linked_list super;
  flup_list_head list;
  int version;
};

struct flup_linked_node {
  flup_list_head node;
  char data[];
};

struct linked_list_iterator {
  flup_iterator_state state;
  struct linked_list_impl* owner;
  flup_linked_node* next;

  int knownVersion;
};

#endif

