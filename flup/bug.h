#ifndef header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h
#define header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h

/**
 * @file 
 * @brief Bug checking functions
 * 
 * Contains bug checking functions which maybe
 * no-op depending in configuration..
 */

#include "flup/attributes.h"

/// @cond
[[noreturn]]
FLUP_PUBLIC
void flup__trigger_bug(const char* file, int line, const char* func);
/// @endcond

/**
 * @brief Trigger bug handler
 *
 * Call this if code encountered inconsistent
 * state which may be a bug. This macro can be
 * disabled.
 */
#define BUG() flup__trigger_bug(__FILE__, __LINE__, __func__)

/**
 * @brief Conditional BUG
 *
 * Call #BUG() if @p x evaluates to true.
 */
#define BUG_ON(x) do { \
  if (x) \
    BUG(); \
} while(0)

#endif

