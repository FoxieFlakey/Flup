#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "flup/attributes.h"
#include "flup/data_structs/tree/btree.h"
#include "flup/concurrency/mutex.h"
#include "flup/core/panic.h"

#include "lib/btree.h"

#define TO_INTERNAL(x) _Generic((x), \
    flup_btree*: (struct flup_btree_internal*) (x) \
  )
#define FROM_INTERNAL(x) _Generic((x), \
    struct flup_btree_internal*: (flup_btree*) (x) \
  )

struct flup_btree_internal {
  flup_mutex* lock;
  struct btree* tree;
};

struct flup_btree_pair {
  uintptr_t key;
  uintptr_t value;
};

static int cmp(const void* _a, const void* _b, void*) {
  const struct flup_btree_pair* a = (const struct flup_btree_pair*) _a;
  const struct flup_btree_pair* b = (const struct flup_btree_pair*) _b;
  
  if (a->key > b->key)
    return 1;
  else if (a->key < b->key)
    return -1;
  return 0;
}

FLUP_PUBLIC
flup_btree* flup_btree_new() {
  struct flup_btree_internal* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (struct flup_btree_internal) {};
  if (!(self->lock = flup_mutex_new()))
    goto failure;
  
  if (!(self->tree = btree_new(sizeof(struct flup_btree_pair), 64, cmp, NULL)))
    goto failure;
  return FROM_INTERNAL(self);

failure:
  btree_free(self->tree);
  return NULL;
}

FLUP_PUBLIC
void flup_btree_free(flup_btree* _self) {
  struct flup_btree_internal* self = TO_INTERNAL(_self);
  btree_free(self->tree);
  flup_mutex_free(self->lock);
  free(self);
}

FLUP_PUBLIC
int flup_btree_insert(flup_btree* _self, uintptr_t key, uintptr_t value) {
  struct flup_btree_internal* self = TO_INTERNAL(_self);
  struct flup_btree_pair pair = {
    .key = key,
    .value = value
  };
  
  flup_mutex_lock(self->lock);
  if (btree_get(self->tree, &pair) != NULL) {
    flup_mutex_unlock(self->lock);
    return -EEXIST;
  }
  
  btree_set(self->tree, &pair);
  int ret = btree_oom(self->tree) ? -ENOMEM : 0;
  
  flup_mutex_unlock(self->lock);
  return ret;
}

FLUP_PUBLIC
int flup_btree_set(flup_btree* _self, uintptr_t key, uintptr_t value) {
  struct flup_btree_internal* self = TO_INTERNAL(_self);
  
  struct flup_btree_pair pair = {
    .key = key,
    .value = value
  };
  
  flup_mutex_lock(self->lock);
  
  btree_set(self->tree, &pair);
  int ret = btree_oom(self->tree) ? -ENOMEM : 0;
  
  flup_mutex_unlock(self->lock);
  return ret;
}

FLUP_PUBLIC
int flup_btree_remove(flup_btree* _self, uintptr_t key) {
  struct flup_btree_internal* self = TO_INTERNAL(_self);
  
  struct flup_btree_pair pair = {
    .key = key
  };
  
  flup_mutex_lock(self->lock);
  if (btree_get(self->tree, &pair) == NULL) {
    flup_mutex_unlock(self->lock);
    return -ENOENT;
  }
  
  btree_delete(self->tree, &pair);
  if (btree_oom(self->tree))
    flup_panic("Not implemented: make sure deletion path is allocation free");
  
  flup_mutex_unlock(self->lock);
  return 0;
}

FLUP_PUBLIC
int flup_btree_get(flup_btree* _self, uintptr_t key, uintptr_t* result) {
  struct flup_btree_internal* self = TO_INTERNAL(_self);
  struct flup_btree_pair pair = {
    .key = key,
    .value = 0
  };
  
  flup_mutex_lock(self->lock);
  const struct flup_btree_pair* item = btree_get(self->tree, &pair);
  flup_mutex_unlock(self->lock);
  if (!item)
    return -ENOENT;
  
  *result = item->value;
  return 0;
}



