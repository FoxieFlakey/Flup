#include <stdint.h>
#include <stddef.h>
#include <xxhash.h>

#include "flup/attributes.h"
#include "flup/hashing/noncrypto/xxhash.h"

FLUP_PUBLIC
uint64_t flup_xxhash3_hash_64bits(const void* data, size_t size) {
  return (uint64_t) XXH3_64bits(data, size);
}

FLUP_PUBLIC
_BitInt(128) flup_xxhash3_hash_128bits(const void* data, size_t size) {
  XXH128_hash_t hash = XXH3_128bits(data, size);
  return ((_BitInt(128)) hash.low64) | ((_BitInt(128)) hash.high64) << 64;
}

