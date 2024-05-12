#ifndef UWU_FE32E8A1_1BAB_41F6_A574_B6F9334D12E0_UWU
#define UWU_FE32E8A1_1BAB_41F6_A574_B6F9334D12E0_UWU

#include "flup/attributes.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/list_head.h"
#include "flup/data_structs/tree/btree.h"
#include "flup/hashing/hash_func.h"

typedef struct flup_string_map {
  flup_mutex* lock;
  
  // Tree going to contain linked list of key/value pairs
  // with same hash (which is used as key to the tree)
  flup_btree* tree;
  flup_hash64_func hashFunction;
  
  flup_list_head allList;
} flup_string_map;

typedef struct flup_string_map_pair {
  flup_list_head node;
  
  flup_hash64 keyHash;
  void* value;
  char key[];
} flup_string_map_pair;

FLUP_PUBLIC
flup_string_map* flup_string_map_new();

FLUP_PUBLIC
flup_string_map* flup_string_map_new_with_hash_func(flup_hash64_func hasher);

FLUP_PUBLIC
void flup_string_map_free(flup_string_map* self);

FLUP_PUBLIC
int flup_string_map_set(flup_string_map* self, const char* key, void* value);

FLUP_PUBLIC
int flup_string_map_get(flup_string_map* self, const char* key, void** value);

FLUP_PUBLIC
int flup_string_map_remove(flup_string_map* self, const char* key, void** value);

#endif
