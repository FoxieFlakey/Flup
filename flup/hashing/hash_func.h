#ifndef UWU_E7D539E6_51C3_48CF_B780_6FDD14CDB5D4_UWU
#define UWU_E7D539E6_51C3_48CF_B780_6FDD14CDB5D4_UWU

#include <stdint.h>
#include <stddef.h>

/**
 * @file
 * @brief Containing typedefs for hash functions
 */

/**
 * @brief A general purpose data to uintptr_t hash function
 *
 * @param data The input data
 * @param size Size of the input data
 *
 * @return uintmax_t sized hash
 */
typedef uintmax_t (*flup_hash_func)(const void* data, size_t size);

#endif
