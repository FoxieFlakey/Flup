#ifndef header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h
#define header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h

#include "flup/core/panic.h" // IWYU pragma: keep

#define BUG() do { \
  flup_panic("BUG: failure at %s:%d/%s()!", __FILE__, __LINE__, __func__); \
} while(0)

#define BUG_ON(x) do { \
  if (x) \
    BUG(); \
} while(0)

#endif

