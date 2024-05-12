#ifndef UWU_FE32E8A1_1BAB_41F6_A574_B6F9334D12E0_UWU
#define UWU_FE32E8A1_1BAB_41F6_A574_B6F9334D12E0_UWU

#include "flup/attributes.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/list_head.h"
#include "flup/data_structs/tree/btree.h"
#include "flup/hashing/hash_func.h"

/**
 * @file
 * @copydoc flup_string_map
 */

/**
 * @brief A string to void* map using tree
 */
typedef struct flup_string_map {
  /// Lock for string map instances
  flup_mutex* lock;
  
  /// Trees containing whole map
  flup_btree* tree;
  /// Hashing function to hash keys
  flup_hash64_func hashFunction;
  
  /// List of all list in the tree
  flup_list_head allList;
} flup_string_map;

/**
 * @brief Represent a pair of key and value
 */
typedef struct flup_string_map_pair {
  /// List node for list of pairs sharing same hash
  flup_list_head node;
  
  /// Hash of keys in @ref flup_string_map_pair.key
  flup_hash64 keyHash;
  /// Opaque void* pointer of key to value mapping
  void* value;
  /// Flexible array containing NUL terminated key
  char key[];
} flup_string_map_pair;

/**
 * @brief Creates new instance of string map
 * 
 * The map is created using xxhash3 (64-bit version) by default
 *
 * @return New instance of string map or NULL on out of memory
 */
FLUP_PUBLIC
flup_string_map* flup_string_map_new();

/**
 * @brief Creates new instance of string map using an hash function
 * 
 * The map is created using user provided hash function
 *
 * @param hasher The custom hashing function provided
 *
 * @return New instance of string map or NULL on out of memory
 */
FLUP_PUBLIC
flup_string_map* flup_string_map_new_with_hash_func(flup_hash64_func hasher);

/**
 * @public @brief Frees an instance of string map
 * @memberof flup_string_map
 *
 * @param self The instance to be removed
 */
FLUP_PUBLIC
void flup_string_map_free(flup_string_map* self);

/**
 * @public @brief Set a key/value pair in map
 * @memberof flup_string_map
 *
 * @param self An instance of string map
 * @param key The key of pair
 * @param value The value of pair
 *
 * @throws ENOMEM Not enough memory
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_string_map_set(flup_string_map* self, const char* key, void* value);

/**
 * @public @brief Gets a value from key in map
 * @memberof flup_string_map
 *
 * @param self An instance of string map
 * @param key The key of pair
 * @param[out] value The resuling value in pair
 *
 * @throws ENOENT Key doesn't map to anything
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_string_map_get(flup_string_map* self, const char* key, void** value);

/**
 * @public @brief Remove a key/value from key in map
 * @memberof flup_string_map
 *
 * @param self An instance of string map
 * @param key The key of pair to be removed
 * @param[out] value The removed value
 *
 * @throws ENOENT Key doesn't map to anything
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_string_map_remove(flup_string_map* self, const char* key, void** value);

#endif
