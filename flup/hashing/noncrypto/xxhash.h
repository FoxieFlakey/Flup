#ifndef UWU_F870BEE6_CD1A_4586_AEFF_D147F7129A15_UWU
#define UWU_F870BEE6_CD1A_4586_AEFF_D147F7129A15_UWU

#include <stddef.h>

#include "flup/attributes.h"
#include "flup/hashing/hash_func.h"

/**
 * @file
 * @brief XXHash non-cryptographic algorithmn
 */

/**
 * @brief Hash using XXH3 algorithmn (64-bit)
 *
 * @param data The data to be hashed
 * @param size The size of data to be hashed
 *
 * @return The hashed data
 */
FLUP_PUBLIC
flup_hash64 flup_xxhash3_hash_64bits(const void* data, size_t size);

/**
 * @brief Hash using XXH3 algorithmn (128-bit)
 *
 * @param data The data to be hashed
 * @param size The size of data to be hashed
 *
 * @return The hashed data
 */
FLUP_PUBLIC
flup_hash128 flup_xxhash3_hash_128bits(const void* data, size_t size);

#endif
