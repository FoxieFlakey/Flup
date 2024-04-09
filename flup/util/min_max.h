#ifndef UWU_AFB2EA92_9823_438F_BD97_D91FE0974C4C_UWU
#define UWU_AFB2EA92_9823_438F_BD97_D91FE0974C4C_UWU

/**
 * @file
 * @brief Min/max operation
 *
 * Contains functions which tell which is larger/smaller
 */

/// @cond
#define FLUP__MIN_MAX_DEFINE(postfix, t) \
  static inline t flup_min_ ## postfix (t a, t b) { \
    return a < b ? a : b; \
  } \
  static inline t flup_max_ ## postfix (t a, t b) { \
    return a > b ? a : b; \
  }

FLUP__MIN_MAX_DEFINE(char, char);
FLUP__MIN_MAX_DEFINE(uchar, unsigned char);
FLUP__MIN_MAX_DEFINE(short, short);
FLUP__MIN_MAX_DEFINE(ushort, unsigned short);
FLUP__MIN_MAX_DEFINE(int, int);
FLUP__MIN_MAX_DEFINE(uint, unsigned int);
FLUP__MIN_MAX_DEFINE(long, long);
FLUP__MIN_MAX_DEFINE(ulong, unsigned long);
FLUP__MIN_MAX_DEFINE(llong, long long);
FLUP__MIN_MAX_DEFINE(ullong, unsigned long long);

#define FLUP__MIN_MAX_SELECT(op, a, b) _Generic((a), \
    char: flup_ ## op ## _char, \
    unsigned char: flup_min_uchar, \
    short: flup_min_short, \
    unsigned short: flup_min_ushort, \
    int: flup_min_int, \
    unsigned int: flup_min_uint, \
    long: flup_min_long, \
    unsigned long: flup_min_ulong, \
    long long: flup_min_llong, \
    unsigned long long: flup_min_ullong \
  )(a, b)
/// @endcond

/**
 * @brief Return smallest value
 *
 * This function return first if first < second or second if
 * second < first
 *
 * @param a The first value
 * @param a The second value
 */
#define flup_min(a, b) FLUP__MIN_MAX_SELECT(min, a, b)

/**
 * @brief Return largest value
 *
 * This function return first if first > second or second if
 * second > first
 *
 * @param a The first value
 * @param a The second value
 */
#define flup_max(a, b) FLUP__MIN_MAX_SELECT(max, a, b)

#endif
