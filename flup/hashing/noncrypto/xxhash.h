#ifndef UWU_F870BEE6_CD1A_4586_AEFF_D147F7129A15_UWU
#define UWU_F870BEE6_CD1A_4586_AEFF_D147F7129A15_UWU

#include <stdint.h>
#include <stddef.h>

#include "flup/attributes.h"

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
uint64_t flup_xxhash3_hash_64bits(const void* data, size_t size);

/**
 * @brief Hash using XXH3 algorithmn (128-bit)
 *
 * @param data The data to be hashed
 * @param size The size of data to be hashed
 *
 * @return The hashed data
 */
FLUP_PUBLIC
_BitInt(128) flup_xxhash3_hash_128bits(const void* data, size_t size);

#endif
