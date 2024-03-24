#ifndef header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h
#define header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h

/**
 * @file 
 * @brief Bug checking functions
 * 
 * Contains bug checking functions which maybe
 * no-op depending in configuration..
 */

#include "flup/core/panic.h" // IWYU pragma: keep

/**
 * @brief Trigger bug handler
 *
 * Call this if code encountered inconsistent
 * state which may be a bug. This macro can be
 * disabled.
 */
#define BUG() do { \
  flup_panic("BUG: failure at %s:%d/%s()!", __FILE__, __LINE__, __func__); \
} while(0)

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

