#ifndef header_1703927662_e8d6a758_cbf1_44e3_85b5_4c0ebfcb26fe_foreach_h
#define header_1703927662_e8d6a758_cbf1_44e3_85b5_4c0ebfcb26fe_foreach_h

/**
 * @file
 * @brief Foreach syntax
 */

#include "flup/compiler.h"
#include "flup/typecheck.h"

/**
 * @brief A macro expanded to "("
 */
#define FLUP_FOREACH_OPEN_PAREN (

/**
 * @brief A macro expanded to ")"
 */
#define FLUP_FOREACH_CLOSE_PAREN )

#if !FLUP_APPLE_BLOCKS_AVAILABLE
static_assert("flup_foreach.h unavailable due Apple Blocks not enabled -w- (hint: -fblocks on Clang compiler)");
#endif

/**
 * @brief A generic type for getting next item
 *
 * @note Block_release won't be called
 *
 * @param genericItem ItemType* casted to void*
 * @return false to signify end of items and true to continue
 */
typedef bool (^flup_foreach_next_block)(void* genericItem);

/**
 * @brief Foreach syntax
 *
 * This acts like for syntax, used like
 * ```c
 * flup_foreach(int, x, integerIteratorBlock)
 *   printf("Integer %d\n", x);
 * ```
 *
 * @param itemType The item type name to iterate with foreach
 * @param itemName The variable name
 * @param nextBlock A non generalized block of @ref flup_foreach_next_block
 */
#define flup_foreach(itemType, itemName, nextBlock, ...) \
  for (itemType itemName __VA_OPT__(= FLUP_FOREACH_OPEN_PAREN) __VA_ARGS__ __VA_OPT__(FLUP_FOREACH_CLOSE_PAREN), *__flup_foreach_block = (void*) FLUP_CHECK_VAR_BLOCK_TYPE((nextBlock), bool, itemType*); \
       ((bool (^)(itemType*)) __flup_foreach_block)(&itemName);)

#endif

