#ifndef header_1703854935_5d95709d_2525_4c6d_bc32_855dcd6772d8_attributes_h
#define header_1703854935_5d95709d_2525_4c6d_bc32_855dcd6772d8_attributes_h

/**
 * @file
 * @brief Macros for GNU C attributes
 * 
 * Header containing various macro for GNU C
 * specific attributes with proper detection.
 */

/**
 * @brief Clang's _Nonnull
 *
 * Expands _Nonnull on supporting compilers.
 */
#define FLUP_NONNULL

/**
 * @brief Clang's _Nullable
 *
 * Expands _Nullable on supporting compilers.
 */
#define FLUP_NULLABLE

/**
 * @brief GNU C's \_\_attribute\_\_
 *
 * Expands __attribute__(@p x) on supporting
 * compilers.
 */
#define FLUP_ATTR(x)

/**
 * @brief Attribute for printf-like functions
 *
 * Allows compiler to check validity of format and arguments
 * 
 * @param fmtOffset Offset to parameter where printf's
 *    format string is where 1 is first argument
 * @param vaStart Offset to where the "..." var arg starts
 *    same as @p fmtOffset, its one based
 */
#define FLUP_ATTR_PRINTF(fmtOffset, vaStart) FLUP_ATTR((format(printf, fmtOffset, vaStart)))

/**
 * @brief Attribute for scanf-like functions
 *
 * Allows compiler to check validity of format and arguments
 *
 * @param fmtOffset Offset to parameter where scanf's
 *    format string is where 1 is first argument
 * @param vaStart Offset to where the "..." var arg starts
 *    same as @p fmtOffset, its one based
 */
#define FLUP_ATTR_SCANF(fmtOffset, vaStart) FLUP_ATTR((format(scanf, fmtOffset, vaStart)))

/**
 * @brief Attribute for "const" function
 *
 * Const function is stricter than pure functions.
 * Functions not allowed to read non-volatile
 * non-const variables it only allowed to read const
 * non volatile variables.
 */
#define FLUP_ATTR_CONST FLUP_ATTR((const))

/**
 * @brief Attribute for "pure" function
 *
 * Pure function is function which perform its
 * operation on its arguments and only reads
 * non volatile memory.
 */
#define FLUP_ATTR_PURE FLUP_ATTR((pure))

/**
 * @brief Tag a pointer to an address space
 *
 * For tagging a compiler during compile time
 * to an address space if the pointer not meant
 * to be dereferenced directly
 *
 * @param n An positive integer of address space ID
 */
#define FLUP_ATTR_ADDRESS_SPACE(n) 

/**
 * @brief Tag a function/variable as used
 *
 * Tag a function as used so that its definition
 * not removed by compiler. Mainly used to prevent
 * Function from being compiled out by optimizer.
 */
#define FLUP_USED FLUP_ATTR((used))

/**
 * @brief Tag a function/variable as maybe unused
 *
 * Tag a function as maybe unused will inform the
 * compiler that function/variable maybe unused and
 * not produce warning.
 */
#define FLUP_UNUSED FLUP_ATTR((unused))

/**
 * @brief Deprecate a function
 *
 * Deprecate a function with a message and suggest
 * replacement.
 *
 * @param msg Message for why its deprecated in string.
 * @param replacedBy Replacement function name in string.
 */
#define FLUP_DEPRECATED(msg, replacedBy) FLUP_ATTR((deprecated(msg, replacedBy)))

/**
 * @def FLUP_PUBLIC
 * @brief Mark a function as public (exported)
 *
 * Expands to proper attributes needed for given
 * platform like "__attribute__(visibility("default")) extern" for
 * GCC and Clang compilers.
 */
#ifndef FLUP_PUBLIC
# define FLUP_PUBLIC FLUP_USED FLUP_ATTR((visibility("default"))) extern
#endif

/**
 * @def FLUP_PUBLIC_VAR
 * @brief Mark a global variable as public (exported)
 *
 * Expands to proper attributes needed for given
 * platform like "__attribute__(visibility("default"))" for
 * GCC and Clang compilers.
 */
#ifndef FLUP_PUBLIC_VAR
# define FLUP_PUBLIC_VAR FLUP_ATTR((visibility("default")))
#endif

#ifdef __GNUC__
# undef FLUP_ATTR
# define FLUP_ATTR(x) __attribute__(x)
#endif

#if __clang__ && __has_attribute(__address_space__)
# undef FLUP_ATTR_ADDRESS_SPACE
# define FLUP_ATTR_ADDRESS_SPACE(n)  FLUP_ATTR((__address_space__(n)))
#endif

#if __clang__
#undef FLUP_NONNULL
#define FLUP_NONNULL _Nonnull

#undef FLUP_NULLABLE
#define FLUP_NULLABLE _Nullable
#endif

#endif

