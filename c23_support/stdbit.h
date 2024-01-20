#ifndef __STDC_VERSION_STDBIT_H__
#define __STDC_VERSION_STDBIT_H__ 202311L

// Some part from : https://www.openwall.com/lists/musl/2023/05/31/60

#include <features.h>
#include <limits.h>

#ifndef __STDC_ENDIAN_BIG__
#define __STDC_ENDIAN_BIG__ __BIG_ENDIAN
#endif
#ifndef __STDC_ENDIAN_LITTLE__
#define __STDC_ENDIAN_LITTLE__ __LITTLE_ENDIAN
#endif
#ifndef __STDC_ENDIAN_NATIVE__
#define __STDC_ENDIAN_NATIVE__ __BYTE_ORDER
#endif

#if !__has_builtin(__builtin_popcountg)
# define __builtin_popcountg(x) ((int) _Generic((x), \
    unsigned char: __builtin_popcount((x) & 0xFF), \
    unsigned short: __builtin_popcount((x) & 0xFFF), \
    unsigned int: __builtin_popcount((x)), \
    unsigned long: __builtin_popcountl((x)), \
    unsigned long long: __builtin_popcountll((x)) \
    ))
#endif

#if !__has_builtin(__builtin_clzg)
# define __builtin_clzg(x, ifZero) ((int) ((x) == ((typeof(x)) 0) ? (int) (ifZero) : _Generic((x), \
    unsigned char: __builtin_clz((x) & 0xFF), \
    unsigned short: __builtin_clz((x) & 0xFFF), \
    unsigned int: __builtin_clz((x)), \
    unsigned long: __builtin_clzl((x)), \
    unsigned long long: __builtin_clzll((x)) \
    )))
#endif

#if !__has_builtin(__builtin_ctzg)
# define __builtin_ctzg(x, ifZero) ((int) ((x) == ((typeof(x)) 0) ? (int) (ifZero) : _Generic((x), \
    unsigned char: __builtin_ctz((x) & 0xFF), \
    unsigned short: __builtin_ctz((x) & 0xFFF), \
    unsigned int: __builtin_ctz((x)), \
    unsigned long: __builtin_ctzl((x)), \
    unsigned long long: __builtin_ctzll((x)) \
    )))
#endif

[[gnu::const]]
static inline unsigned int stdc_count_ones_uc(unsigned char val) {
  return (unsigned int) __builtin_popcountg(val);
}
[[gnu::const]]
static inline unsigned int stdc_count_ones_us(unsigned short val) {
  return (unsigned int) __builtin_popcountg(val);
}
[[gnu::const]]
static inline unsigned int stdc_count_ones_ui(unsigned int val) {
  return (unsigned int) __builtin_popcountg(val);
}
[[gnu::const]]
static inline unsigned int stdc_count_ones_ul(unsigned long val) {
  return (unsigned int) __builtin_popcountg(val);
}
[[gnu::const]]
static inline unsigned int stdc_count_ones_ull(unsigned long long val) {
  return (unsigned int) __builtin_popcountg(val);
}

[[gnu::const]]
static inline unsigned int stdc_count_zeros_uc(unsigned char val) {
  return (unsigned int) __builtin_popcountg((unsigned char) ~val);
}
[[gnu::const]]
static inline unsigned int stdc_count_zeros_us(unsigned short val) {
  return (unsigned int) __builtin_popcountg((unsigned short) ~val);
}
[[gnu::const]]
static inline unsigned int stdc_count_zeros_ui(unsigned int val) {
  return (unsigned int) __builtin_popcountg(~val);
}
[[gnu::const]]
static inline unsigned int stdc_count_zeros_ul(unsigned long val) {
  return (unsigned int) __builtin_popcountg(~val);
}
[[gnu::const]]
static inline unsigned int stdc_count_zeros_ull(unsigned long long val) {
  return (unsigned int) __builtin_popcountg(~val);
}

[[gnu::const]]
static inline unsigned int stdc_leading_zeros_uc(unsigned char val) {
  return (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_zeros_us(unsigned short val) {
  return  (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_zeros_ui(unsigned int val) {
  return (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_zeros_ul(unsigned long val) {
  return (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_zeros_ull(unsigned long long val) {
  return (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}

[[gnu::const]]
static inline unsigned int stdc_leading_ones_uc(unsigned char val) {
  return (unsigned int) __builtin_clzg((unsigned char) ~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_ones_us(unsigned short val) {
  return (unsigned int) __builtin_clzg((unsigned short) ~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_ones_ui(unsigned int val) {
  return (unsigned int) __builtin_clzg(~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_ones_ul(unsigned long val) {
  return (unsigned int) __builtin_clzg(~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_leading_ones_ull(unsigned long long val) {
  return (unsigned int) __builtin_clzg(~val, sizeof(val) * CHAR_BIT);
}

[[gnu::const]]
static inline unsigned int stdc_trailing_zeros_uc(unsigned char val) {
  return (unsigned int) __builtin_ctzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_zeros_us(unsigned short val) {
  return (unsigned int) __builtin_ctzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_zeros_ui(unsigned int val) {
  return (unsigned int) __builtin_ctzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_zeros_ul(unsigned long val) {
  return (unsigned int) __builtin_ctzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_zeros_ull(unsigned long long val) {
  return (unsigned int) __builtin_ctzg(val, sizeof(val) * CHAR_BIT);
}

[[gnu::const]]
static inline unsigned int stdc_trailing_ones_uc(unsigned char val) {
  return (unsigned int) __builtin_ctzg((unsigned char) ~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_ones_us(unsigned short val) {
  return (unsigned int) __builtin_ctzg((unsigned short) ~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_ones_ui(unsigned int val) {
  return (unsigned int) __builtin_ctzg(~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_ones_ul(unsigned long val) {
  return (unsigned int) __builtin_ctzg(~val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_trailing_ones_ull(unsigned long long val) {
  return (unsigned int) __builtin_ctzg(~val, sizeof(val) * CHAR_BIT);
}

[[gnu::const]]
static inline unsigned int stdc_first_leading_zero_uc(unsigned char val) {
  return (unsigned int) (__builtin_clzg((unsigned char) ~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_zero_us(unsigned short val) {
  return (unsigned int) (__builtin_clzg((unsigned short) ~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_zero_ui(unsigned int val) {
  return (unsigned int) (__builtin_clzg(~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_zero_ul(unsigned long val) {
  return (unsigned int) (__builtin_clzg(~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_zero_ull(unsigned long long val) {
  return (unsigned int) (__builtin_clzg(~val, -1) + 1);
}

[[gnu::const]]
static inline unsigned int stdc_first_leading_one_uc(unsigned char val) {
  return (unsigned int) (__builtin_clzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_one_us(unsigned short val) {
  return (unsigned int) (__builtin_clzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_one_ui(unsigned int val) {
  return (unsigned int) (__builtin_clzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_one_ul(unsigned long val) {
  return (unsigned int) (__builtin_clzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_leading_one_ull(unsigned long long val) {
  return (unsigned int) (__builtin_clzg(val, -1) + 1);
}

[[gnu::const]]
static inline unsigned int stdc_first_trailing_one_uc(unsigned char val) {
  return (unsigned int) (__builtin_ctzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_one_us(unsigned short val) {
  return (unsigned int) (__builtin_ctzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_one_ui(unsigned int val) {
  return (unsigned int) (__builtin_ctzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_one_ul(unsigned long val) {
  return (unsigned int) (__builtin_ctzg(val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_one_ull(unsigned long long val) {
  return (unsigned int) (__builtin_ctzg(val, -1) + 1);
}

[[gnu::const]]
static inline unsigned int stdc_first_trailing_zero_uc(unsigned char val) {
  return (unsigned int) (__builtin_ctzg((unsigned char) ~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_zero_us(unsigned short val) {
  return (unsigned int) (__builtin_ctzg((unsigned short) ~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_zero_ui(unsigned int val) {
  return (unsigned int) (__builtin_ctzg(~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_zero_ul(unsigned long val) {
  return (unsigned int) (__builtin_ctzg(~val, -1) + 1);
}
[[gnu::const]]
static inline unsigned int stdc_first_trailing_zero_ull(unsigned long long val) {
  return (unsigned int) (__builtin_ctzg(~val, -1 ) + 1);
}

[[gnu::const]]
static inline bool stdc_has_single_bit_uc(unsigned char val) {
  return (unsigned int) __builtin_popcountg(val) == 1;
}
[[gnu::const]]
static inline bool stdc_has_single_bit_us(unsigned short val) {
  return (unsigned int) __builtin_popcountg(val) == 1;
}
[[gnu::const]]
static inline bool stdc_has_single_bit_ui(unsigned int val) {
  return (unsigned int) __builtin_popcountg(val) == 1;
}
[[gnu::const]]
static inline bool stdc_has_single_bit_ul(unsigned long val) {
  return (unsigned int) __builtin_popcountg(val) == 1;
}
[[gnu::const]]
static inline bool stdc_has_single_bit_ull(unsigned long long val) {
  return (unsigned int) __builtin_popcountg(val) == 1;
}

[[gnu::const]]
static inline unsigned int stdc_bit_width_uc(unsigned char val) {
  return (sizeof(val) * CHAR_BIT) - (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_bit_width_us(unsigned short val) {
  return (sizeof(val) * CHAR_BIT) - (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_bit_width_ui(unsigned int val) {
  return (sizeof(val) * CHAR_BIT) - (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_bit_width_ul(unsigned long val) {
  return (sizeof(val) * CHAR_BIT) - (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}
[[gnu::const]]
static inline unsigned int stdc_bit_width_ull(unsigned long long val) {
  return (sizeof(val) * CHAR_BIT) - (unsigned int) __builtin_clzg(val, sizeof(val) * CHAR_BIT);
}

[[gnu::const]]
static inline unsigned char stdc_bit_floor_uc(unsigned char val) {
  return val == 0 ? (typeof(val)) 0 : (typeof(val)) (1 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned short stdc_bit_floor_us(unsigned short val) {
  return val == 0 ? (typeof(val)) 0 : (typeof(val)) (1 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned int stdc_bit_floor_ui(unsigned int val) {
  return val == 0 ? (typeof(val)) 0 : (typeof(val)) (1 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned long stdc_bit_floor_ul(unsigned long val) {
  return val == 0 ? (typeof(val)) 0 : (typeof(val)) (1 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned long long stdc_bit_floor_ull(unsigned long long val) {
  return val == 0 ? (typeof(val)) 0 : (typeof(val)) (1 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}

[[gnu::const]]
static inline unsigned char stdc_bit_ceil_uc(unsigned char val) {
  return val <= 1 ? (typeof(val)) 1 : (typeof(val)) (2 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned short stdc_bit_ceil_us(unsigned short val) {
  return val <= 1 ? (typeof(val)) 1 : (typeof(val)) (2 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned int stdc_bit_ceil_ui(unsigned int val) {
  return val <= 1 ? (typeof(val)) 1 : (typeof(val)) (2 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned long stdc_bit_ceil_ul(unsigned long val) {
  return val <= 1 ? (typeof(val)) 1 : (typeof(val)) (2 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}
[[gnu::const]]
static inline unsigned long long stdc_bit_ceil_ull(unsigned long long val) {
  return val <= 1 ? (typeof(val)) 1 : (typeof(val)) (2 << ((sizeof(val) * CHAR_BIT) - 1 - (unsigned int) __builtin_clzg(val, 0)));
}

#endif

