#ifndef UWU_D5452213_C2E3_44D8_B8A2_C419AE576836_UWU
#define UWU_D5452213_C2E3_44D8_B8A2_C419AE576836_UWU

#include "flup/compile_time/must_be_array.h"

/**
 * @file
 * @brief Contains various convenient macro for arrays in C
 */

/**
 * @brief Calculate the size of array
 *
 * @param array The array
 */
#define ARRAY_SIZE(array) (sizeof((array)) / sizeof((array)[0]) + flup_must_be_array(flup___Cannot_use_ARRAY_SIZE_on_pointer, arr))

/// @cond
typedef void flup___Cannot_use_ARRAY_SIZE_on_pointer;
/// @endcond

#endif
