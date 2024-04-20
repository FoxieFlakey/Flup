#ifndef UWU_E24E67EF_21D2_449F_958A_D765C2277EC5_UWU
#define UWU_E24E67EF_21D2_449F_958A_D765C2277EC5_UWU

/**
 * @file
 * @brief Expression checks macros
 */

/// @cond
typedef void flup___Cannot_happen;
/// @endcond

/**
 * @brief Evaluates to 0 if expr true or cause compile error
 *
 * @param err The type of error to be generated (must be typedef of void)
 * @param expr Expression to check
 *
 * @return 0 on success, or well compile error
 */
# define flup_must_be_true(err, expr) _Generic((unsigned _BitInt(!!(expr) + 1)) 0, \
  unsigned _BitInt(2): 0, \
  default: (err) 0 \
)

#endif
