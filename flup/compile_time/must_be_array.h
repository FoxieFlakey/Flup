#ifndef UWU_C68FB984_3262_480A_8027_FA19D7ECB1B0_UWU
#define UWU_C68FB984_3262_480A_8027_FA19D7ECB1B0_UWU

/**
 * @file
 * @copydoc flup_must_be_array
 */

/**
 * @brief Evaluates to 0 if array is an array or cause compile error
 *
 * @param err Error to compile error with
 * @param array The array to be checked
 *
 * @return 0 on success, or well compile error
 */
# define flup_must_be_array(err, array) \
  _Generic(&(array), \
    typeof((array)[0])**: (err) 0, \
    default: 0 \
  )

#if defined(__GNUC__) && defined(__has_builtin) && __has_builtin(__builtin_types_compatible_p)
# undef flup_must_be_array

# include "flup/compile_time/must_be_true.h"
# define flup_must_be_array(err, array) flup__same_type(err, (array), &(array)[0])
# define flup__same_type(err, a, b) flup_must_be_true(err, __builtin_types_compatible_p(typeof(a), typeof(b)) == 0)
#endif

#endif
