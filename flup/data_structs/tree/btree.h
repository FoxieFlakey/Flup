#ifndef UWU_EAD14DDB_96EB_4629_8AD4_DF1CA416BD95_UWU
#define UWU_EAD14DDB_96EB_4629_8AD4_DF1CA416BD95_UWU

/**
 * @file
 * @copydoc flup_btree
 */

#include <stdint.h>

#include "flup/attributes.h"

/**
 * @brief BTree API
 *
 * Currently its opaque as this just wrapper for https://github.com/tidwall/btree.c
 * because B tree is somewhat too complex to write in code instead showing example
 */
typedef struct alignas(alignof(void*)) flup_btree {} flup_btree;

/**
 * @public @brief Allocate new instance of @ref flup_btree
 *
 * @return New instance of B tree
 */
FLUP_PUBLIC
flup_btree* flup_btree_new();

/**
 * @public @brief Frees an instance of ref flup_btree
 * @memberof flup_btree
 * 
 * @param self The instance to be freed
 */
FLUP_PUBLIC
void flup_btree_free(flup_btree* self);

/**
 * @public @brief Insert new key and value pair
 * @memberof flup_btree
 *
 * @param self The instance to add a pair to
 * @param key The key of pair
 * @param value The value of pair
 *
 * @throws EEXIST A key value pair already exist
 * @throws ENOMEM Cannot allocate memory
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_btree_insert(flup_btree* self, uintptr_t key, uintptr_t value);

/**
 * @public @brief Sets a key and value pair
 * @memberof flup_btree
 *
 * Same as @ref flup_btree.flup_btree_insert but overwrites if exist
 *
 * @param self The instance to add a pair to
 * @param key The key of pair
 * @param value The value of pair
 *
 * @throws ENOMEM Cannot allocate memory
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_btree_set(flup_btree* self, uintptr_t key, uintptr_t value);

/**
 * @public @brief Removes new key and value pair
 * @memberof flup_btree
 *
 * @param self The instance to remove a pair from
 * @param key The key of pair
 *
 * @throws ENOENT A key don't exist
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_btree_remove(flup_btree* self, uintptr_t key);

/**
 * @public @brief Retrieves a value from key
 * @memberof flup_btree
 *
 * Find a value with key of @p key and sets value to @p result
 *
 * @param self The instance to remove a pair from
 * @param key The key of pair
 * @param[out] result The resulting value
 *
 * @throws ENOENT A key don't exist
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_btree_get(flup_btree* self, uintptr_t key, uintptr_t* result);

#endif
