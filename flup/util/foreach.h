#ifndef header_1703927662_e8d6a758_cbf1_44e3_85b5_4c0ebfcb26fe_foreach_h
#define header_1703927662_e8d6a758_cbf1_44e3_85b5_4c0ebfcb26fe_foreach_h

#include "flup/compiler.h"
#include "flup/typecheck.h"

#define FLUP_OPEN_PAREN (
#define FLUP_CLOSE_PAREN )

// nextBlock is type of bool (^)(itemType* item)
// return false if nothing else to continue else 
// true and Block_release won't be called
#define flup_foreach(itemType, itemName, nextBlock, ...) \
  for (itemType itemName __VA_OPT__(= FLUP_OPEN_PAREN) __VA_ARGS__ __VA_OPT__(FLUP_CLOSE_PAREN), *__flup_foreach_block = (void*) FLUP_CHECK_VAR_FUNC_TYPE((nextBlock), bool, itemType*); \
       ((bool (^)(itemType*)) __flup_foreach_block)(&itemName);)

#if !FLUP_APPLE_BLOCKS_AVAILABLE
static_assert("flup_foreach.h unavailable due Apple Blocks not enabled -w- (hint: -fblocks on Clang compiler)");
#endif

#endif

