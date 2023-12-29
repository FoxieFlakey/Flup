#ifndef header_1703854935_5d95709d_2525_4c6d_bc32_855dcd6772d8_attributes_h
#define header_1703854935_5d95709d_2525_4c6d_bc32_855dcd6772d8_attributes_h

#ifdef __GNUC__
# define ATTRIBUTE(x) __attribute__(x)
#else
# define ATTRIBUTE(x)
#endif

#define ATTRIBUTE_PRINTF(fmtOffset, vaStart) ATTRIBUTE((format(printf, fmtOffset, vaStart)))
#define ATTRIBUTE_SCANF(fmtOffset, vaStart) ATTRIBUTE((format(scanf, fmtOffset, vaStart)))

// Properly tag function exported functions with this to ensure LTO not removing them
#define ATTRIBUTE_USED() ATTRIBUTE((used))

#define ATTRIBUTE_PURE() ATTRIBUTE((pure))
#define ATTRIBUTE_CONST() ATTRIBUTE((const))

#if __clang__ && __has_attribute(__address_space__)
# define ATTRIBUTE_ADDRESS_SPACE(n)  ATTRIBUTE((__address_space__(n)))
#elif
# define ATTRIBUTE_ADDRESS_SPACE(n) 
#endif

#endif

