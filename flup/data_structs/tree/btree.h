#ifndef UWU_EAD14DDB_96EB_4629_8AD4_DF1CA416BD95_UWU
#define UWU_EAD14DDB_96EB_4629_8AD4_DF1CA416BD95_UWU

#include <stdint.h>

#include "flup/attributes.h"

// Right now its using under the hood
// https://github.com/tidwall/btree.c

typedef struct flup_btree flup_btree;

FLUP_PUBLIC
flup_btree* flup_btree_new();

FLUP_PUBLIC
void flup_btree_free(flup_btree* self);

FLUP_PUBLIC
int flup_btree_insert(flup_btree* self, uintptr_t key, uintptr_t value);

FLUP_PUBLIC
int flup_btree_set(flup_btree* self, uintptr_t key, uintptr_t value);

FLUP_PUBLIC
int flup_btree_remove(flup_btree* self, uintptr_t key);

FLUP_PUBLIC
int flup_btree_get(flup_btree* self, uintptr_t key, uintptr_t* result);

#endif
