#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>

#include "flup/bug.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/string_map.h"
#include "flup/attributes.h"
#include "flup/data_structs/list_head.h"
#include "flup/data_structs/tree/btree.h"
#include "flup/hashing/hash_func.h"
#include "flup/hashing/noncrypto/xxhash.h"

struct pair_list {
  flup_list_head node;
  flup_list_head list;
};

FLUP_PUBLIC
int flup_string_map_set(flup_string_map* self, const char* key, void* value) {
  flup_mutex_lock(self->lock);
  size_t keyLen = strlen(key);
  flup_hash64 keyHash = self->hashFunction(key, keyLen);
  uintptr_t pairListAddr;
  struct pair_list* pairsList;
  if (flup_btree_get(self->tree, (uintptr_t) keyHash, &pairListAddr) == 0) {
    pairsList = (struct pair_list*) pairListAddr;
    goto no_need_to_create_list;
  }
  
  pairsList = malloc(sizeof(*pairsList));
  if (!pairsList)
    goto out_of_memory_error;
  flup_list_head_init(&pairsList->list);
  flup_list_add_head(&self->allList, &pairsList->node);
  
  int ret = flup_btree_insert(self->tree, keyHash, (uintptr_t) pairsList);
  BUG_ON(ret == -EEXIST);
  if (ret < 0)
    goto out_of_memory_error;

no_need_to_create_list:;
  flup_list_head* current;
  flup_string_map_pair* pair;
  flup_list_for_each(&pairsList->list, current) {
    flup_string_map_pair* pairCurent = flup_list_entry(current, flup_string_map_pair, node);
    if (strcmp(pairCurent->key, key) == 0) {
      pair = pairCurent;
      goto no_need_to_insert_new_pair;
    }
  }
  
  pair = malloc(sizeof(*pair) + keyLen + 1);
  if (!pair)
    goto out_of_memory_error;
  
  pair->keyHash = keyHash;
  pair->value = value;
  memcpy(pair->key, key, keyLen + 1);
  flup_list_add_head(&pairsList->list, &pair->node);

no_need_to_insert_new_pair: 
  BUG_ON(pair->keyHash != keyHash);
  pair->value = value;
  flup_mutex_unlock(self->lock);
  return 0;

out_of_memory_error:
  flup_mutex_unlock(self->lock);
  return -ENOMEM;
}

static int commonGet(flup_string_map* self, const char* key, void** value, bool doRemove) {
  flup_mutex_lock(self->lock);
  size_t keyLen = strlen(key);
  flup_hash64 keyHash = self->hashFunction(key, keyLen);
  uintptr_t pairListAddr;
  struct pair_list* pairsList;
  if (flup_btree_get(self->tree, (uintptr_t) keyHash, &pairListAddr) < 0)
    return -ENOENT;
  
  pairsList = (struct pair_list*) pairListAddr;
  flup_list_head* current;
  flup_string_map_pair* currentPair;
  flup_list_for_each(&pairsList->list, current) {
    currentPair = flup_list_entry(current, flup_string_map_pair, node);
    if (strcmp(currentPair->key, key) == 0) {
      BUG_ON(currentPair->keyHash != keyHash);
      if (value)
        *value = currentPair->value;
      break;
    }
  }
  
  if (doRemove) {
    flup_list_del(current);
    if (flup_list_is_empty(&pairsList->list)) {
      int ret = flup_btree_remove(self->tree, keyHash);
      BUG_ON(ret != 0);
      flup_list_del(&pairsList->node);
      free(pairsList);
    }
    free(currentPair);
  }
  flup_mutex_unlock(self->lock);
  return 0;
}

FLUP_PUBLIC
int flup_string_map_get(flup_string_map* self, const char* key, void** value) {
  return commonGet(self, key, value, false);
}

FLUP_PUBLIC
int flup_string_map_remove(flup_string_map* self, const char* key, void** value) {
  return commonGet(self, key, value, true);
}

FLUP_PUBLIC
flup_string_map* flup_string_map_new() {
  return flup_string_map_new_with_hash_func(flup_xxhash3_hash_64bits);
}

FLUP_PUBLIC
flup_string_map* flup_string_map_new_with_hash_func(flup_hash64_func hasher) {
  flup_string_map* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_string_map) {
    .hashFunction = hasher,
    .allList = FLUP_LIST_HEAD_INIT(self->allList)
  };
  
  if (!(self->lock = flup_mutex_new()))
    goto failure;
  if (!(self->tree = flup_btree_new()))
    goto failure;
  return self;

failure:
  flup_string_map_free(self);
  return NULL;
}

FLUP_PUBLIC
void flup_string_map_free(flup_string_map* self) {
  if (!self)
    return;
  
  flup_list_head* current;
  flup_list_head* next;
  flup_list_for_each_safe(&self->allList, current, next) {
    struct pair_list* pairList = flup_list_entry(current, struct pair_list, node);
    
    flup_list_head* currentPair;
    flup_list_head* nextPair;
    flup_list_for_each_safe(&pairList->list, currentPair, nextPair) {
      flup_string_map_pair* pair = flup_list_entry(currentPair, flup_string_map_pair, node);
      free(pair);
    }
    free(pairList);
  }
  flup_btree_free(self->tree);
  flup_mutex_free(self->lock);
  free(self);
}

