#ifndef header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h
#define header_1703857356_95d637e6_21b9_42dc_a579_ad75ab83dee1_bug_h

#define BUG() do { \
  \
} while(0)

#define BUG_ON(x) do { \
  if (x) \
    BUG(); \
} while(0)

#endif

