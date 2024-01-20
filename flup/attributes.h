#ifndef header_1703854935_5d95709d_2525_4c6d_bc32_855dcd6772d8_attributes_h
#define header_1703854935_5d95709d_2525_4c6d_bc32_855dcd6772d8_attributes_h

#ifdef __GNUC__
# define FLUP_ATTR(x) __attribute__(x)
#else
# define FLUP_ATTR(x)
#endif

#define FLUP_ATTR_PRINTF(fmtOffset, vaStart) FLUP_ATTR((format(printf, fmtOffset, vaStart)))
#define FLUP_ATTR_SCANF(fmtOffset, vaStart) FLUP_ATTR((format(scanf, fmtOffset, vaStart)))
#define FLUP_ATTR_PURE FLUP_ATTR((pure))
#define FLUP_ATTR_CONST FLUP_ATTR((const))

// Properly tag function exported functions with this to ensure LTO not removing them
#define FLUP_USED FLUP_ATTR((used))
#define FLUP_UNUSED FLUP_ATTR((unused))

#define FLUP_DEPRECATED(msg, replacedBy) FLUP_ATTR((deprecated(msg, replacedBy)))

#ifndef FLUP_PUBLIC
# define FLUP_PUBLIC FLUP_USED FLUP_ATTR((visibility("default"))) extern
#endif

#ifndef FLUP_PUBLIC_VAR
# define FLUP_PUBLIC_VAR FLUP_ATTR((visibility("default")))
#endif

#if __clang__ && __has_attribute(__address_space__)
# define FLUP_ATTR_ADDRESS_SPACE(n)  FLUP_ATTR((__address_space__(n)))
#else
# define FLUP_ATTR_ADDRESS_SPACE(n) 
#endif

#if __clang__
#define FLUP_NONNULL _Nonnull
#define FLUP_NULLABLE _Nullable
#else
#define FLUP_NONNULL
#define FLUP_NULLABLE
#endif

#endif

