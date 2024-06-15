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
    unsigned char: flup_ ## op ## _uchar, \
    short: flup_ ## op ## _short, \
    unsigned short: flup_ ## op ## _ushort, \
    int: flup_ ## op ## _int, \
    unsigned int: flup_ ## op ## _uint, \
    long: flup_ ## op ## _long, \
    unsigned long: flup_ ## op ## _ulong, \
    long long: flup_ ## op ## _llong, \
    unsigned long long: flup_ ## op ## _ullong \
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
