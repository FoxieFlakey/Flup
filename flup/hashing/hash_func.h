#ifndef UWU_E7D539E6_51C3_48CF_B780_6FDD14CDB5D4_UWU
#define UWU_E7D539E6_51C3_48CF_B780_6FDD14CDB5D4_UWU

#include <stdint.h>
#include <stddef.h>

/**
 * @file
 * @brief Containing typedefs for hash functions
 */

/// @{
/// Type def for each hash size (castable to corresponding uint<X>_t type)
typedef uint32_t flup_hash32;
typedef uint64_t flup_hash64;
/// @}
/// @{
/// Type def for each hash size (castable to corresponding uint<X>_t type)
/// @return
typedef _BitInt(128) flup_hash128;
typedef _BitInt(256) flup_hash256;
typedef _BitInt(512) flup_hash512;
/// @}

/**
 * @brief A general purpose data to 64-bit hash function
 *
 * @param data The input data
 * @param size Size of the input data
 *
 * @return uint64_t sized hash
 */
typedef flup_hash64 (*flup_hash64_func)(const void* data, size_t size);

#endif
