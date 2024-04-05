// File from https://gitlab.com/Kamcuk/ckd

/**
 * @file
 * @author Kamil Cukrowski <kamilcukrowski@gmail.com>
 * @date 2021-06-19
 * @copyright 2022 Kamil Cukrowski <kamilcukrowski@gmail.com>
 * SPDX-FileCopyrightText: 2022 Kamil Cukrowski <kamilcukrowski@gmail.com>
 * SPDX-License-Identifier: MIT
 */
// Header [[[
#ifndef CKDINT_H_
#define CKDINT_H_

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef _ckd_static
/// @def _ckd_static
#ifdef CKD_COVERAGE
#define _ckd_static  static __attribute__((__noinline__)) __attribute__((__used__))
#else
#define _ckd_static  static inline
#endif
#endif
/// @def _ckd_fchpnt(x)
/// @brief Function attribute for functions that return value and change pointer.
/// @def _ckd_fconst
/// @brief Function attribute for functions that only return value.
#if __GNUC__
#define _ckd_fconst     _ckd_static __attribute__((__warn_unused_result__)) __attribute__((__const__))
#if __GNUC__ >= 10 && !__INTEL_COMPILER
#define _ckd_fchpnt(x)  _ckd_static __attribute__((__warn_unused_result__)) __attribute__((__access__(__write_only__, x)))
#else
#define _ckd_fchpnt(x)  _ckd_static __attribute__((__warn_unused_result__))
#endif
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ > 20230000L
#define _ckd_fconst     _ckd_static [[__nodiscard__]]
#define _ckd_fchpnt(x)  _ckd_static [[__nodiscard__]]
#else
#define _ckd_fconst     _ckd_static
#define _ckd_fchpnt(x)  _ckd_static
#endif

#define CKD_VERSION_DESCRIBE  v0.1.5
#define CKD_VERSION           0x105

// ]]]
// Declare checked integer types [[[
/// @brief A checked integer type for storing value of type char.
typedef struct {
	/// @brief The stored value.
	char _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_char_t;
/// @brief A checked integer type for storing value of type signed char.
typedef struct {
	/// @brief The stored value.
	signed char _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_schar_t;
/// @brief A checked integer type for storing value of type unsigned char.
typedef struct {
	/// @brief The stored value.
	unsigned char _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_uchar_t;
/// @brief A checked integer type for storing value of type short.
typedef struct {
	/// @brief The stored value.
	short _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_shrt_t;
/// @brief A checked integer type for storing value of type unsigned short.
typedef struct {
	/// @brief The stored value.
	unsigned short _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_ushrt_t;
/// @brief A checked integer type for storing value of type int.
typedef struct {
	/// @brief The stored value.
	int _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_int_t;
/// @brief A checked integer type for storing value of type unsigned int.
typedef struct {
	/// @brief The stored value.
	unsigned int _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_uint_t;
/// @brief A checked integer type for storing value of type long.
typedef struct {
	/// @brief The stored value.
	long _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_long_t;
/// @brief A checked integer type for storing value of type unsigned long.
typedef struct {
	/// @brief The stored value.
	unsigned long _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_ulong_t;
/// @brief A checked integer type for storing value of type long long.
typedef struct {
	/// @brief The stored value.
	long long _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_llong_t;
/// @brief A checked integer type for storing value of type unsigned long long.
typedef struct {
	/// @brief The stored value.
	unsigned long long _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_ullong_t;
/// @brief A checked integer type for storing value of type __int128.
typedef struct {
	/// @brief The stored value.
	__int128 _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_int128_t;
/// @brief A checked integer type for storing value of type unsigned __int128.
typedef struct {
	/// @brief The stored value.
	unsigned __int128 _ckd_Value;
	/// @brief The overflow flag.
	bool _ckd_Overflow;
} ckd_uint128_t;
// ]]]
// Standard integer types aliases [[[

/// @def ckd_intmax_t
/// @brief Checked integer type ckd_intmax_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_intmax_t  /* implementation defined type */
#else
#ifdef INTMAX_MAX
#if INTMAX_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_intmax_t;
#elif INTMAX_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_intmax_t;
#elif INTMAX_MAX == INT_MAX
typedef ckd_int_t ckd_intmax_t;
#elif INTMAX_MAX == LONG_MAX
typedef ckd_long_t ckd_intmax_t;
#elif INTMAX_MAX == LLONG_MAX
typedef ckd_llong_t ckd_intmax_t;
#endif
#endif
#endif
/// @def ckd_uintmax_t
/// @brief Checked integer type ckd_uintmax_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uintmax_t  /* implementation defined type */
#else
#ifdef UINTMAX_MAX
#if UINTMAX_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uintmax_t;
#elif UINTMAX_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uintmax_t;
#elif UINTMAX_MAX == UINT_MAX
typedef ckd_uint_t ckd_uintmax_t;
#elif UINTMAX_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uintmax_t;
#elif UINTMAX_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uintmax_t;
#endif
#endif
#endif
/// @def ckd_size_t
/// @brief Checked integer type ckd_size_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_size_t  /* implementation defined type */
#else
#ifdef SIZE_MAX
#if SIZE_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_size_t;
#elif SIZE_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_size_t;
#elif SIZE_MAX == UINT_MAX
typedef ckd_uint_t ckd_size_t;
#elif SIZE_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_size_t;
#elif SIZE_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_size_t;
#endif
#endif
#endif
/// @def ckd_ptrdiff_t
/// @brief Checked integer type ckd_ptrdiff_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_ptrdiff_t  /* implementation defined type */
#else
#ifdef PTRDIFF_MAX
#if PTRDIFF_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_ptrdiff_t;
#elif PTRDIFF_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_ptrdiff_t;
#elif PTRDIFF_MAX == INT_MAX
typedef ckd_int_t ckd_ptrdiff_t;
#elif PTRDIFF_MAX == LONG_MAX
typedef ckd_long_t ckd_ptrdiff_t;
#elif PTRDIFF_MAX == LLONG_MAX
typedef ckd_llong_t ckd_ptrdiff_t;
#endif
#endif
#endif
/// @def ckd_intptr_t
/// @brief Checked integer type ckd_intptr_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_intptr_t  /* implementation defined type */
#else
#ifdef INTPTR_MAX
#if INTPTR_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_intptr_t;
#elif INTPTR_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_intptr_t;
#elif INTPTR_MAX == INT_MAX
typedef ckd_int_t ckd_intptr_t;
#elif INTPTR_MAX == LONG_MAX
typedef ckd_long_t ckd_intptr_t;
#elif INTPTR_MAX == LLONG_MAX
typedef ckd_llong_t ckd_intptr_t;
#endif
#endif
#endif
/// @def ckd_uintptr_t
/// @brief Checked integer type ckd_uintptr_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uintptr_t  /* implementation defined type */
#else
#ifdef UINTPTR_MAX
#if UINTPTR_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uintptr_t;
#elif UINTPTR_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uintptr_t;
#elif UINTPTR_MAX == UINT_MAX
typedef ckd_uint_t ckd_uintptr_t;
#elif UINTPTR_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uintptr_t;
#elif UINTPTR_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uintptr_t;
#endif
#endif
#endif
/// @def ckd_int8_t
/// @brief Checked integer type ckd_int8_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int8_t  /* implementation defined type */
#else
#ifdef INT8_MAX
#if INT8_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int8_t;
#elif INT8_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int8_t;
#elif INT8_MAX == INT_MAX
typedef ckd_int_t ckd_int8_t;
#elif INT8_MAX == LONG_MAX
typedef ckd_long_t ckd_int8_t;
#elif INT8_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int8_t;
#endif
#endif
#endif
/// @def ckd_uint8_t
/// @brief Checked integer type ckd_uint8_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint8_t  /* implementation defined type */
#else
#ifdef UINT8_MAX
#if UINT8_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint8_t;
#elif UINT8_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint8_t;
#elif UINT8_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint8_t;
#elif UINT8_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint8_t;
#elif UINT8_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint8_t;
#endif
#endif
#endif
/// @def ckd_int_least8_t
/// @brief Checked integer type ckd_int_least8_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_least8_t  /* implementation defined type */
#else
#ifdef INT_LEAST8_MAX
#if INT_LEAST8_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_least8_t;
#elif INT_LEAST8_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_least8_t;
#elif INT_LEAST8_MAX == INT_MAX
typedef ckd_int_t ckd_int_least8_t;
#elif INT_LEAST8_MAX == LONG_MAX
typedef ckd_long_t ckd_int_least8_t;
#elif INT_LEAST8_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_least8_t;
#endif
#endif
#endif
/// @def ckd_uint_least8_t
/// @brief Checked integer type ckd_uint_least8_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_least8_t  /* implementation defined type */
#else
#ifdef UINT_LEAST8_MAX
#if UINT_LEAST8_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_least8_t;
#elif UINT_LEAST8_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_least8_t;
#elif UINT_LEAST8_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_least8_t;
#elif UINT_LEAST8_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_least8_t;
#elif UINT_LEAST8_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_least8_t;
#endif
#endif
#endif
/// @def ckd_int_fast8_t
/// @brief Checked integer type ckd_int_fast8_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_fast8_t  /* implementation defined type */
#else
#ifdef INT_FAST8_MAX
#if INT_FAST8_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_fast8_t;
#elif INT_FAST8_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_fast8_t;
#elif INT_FAST8_MAX == INT_MAX
typedef ckd_int_t ckd_int_fast8_t;
#elif INT_FAST8_MAX == LONG_MAX
typedef ckd_long_t ckd_int_fast8_t;
#elif INT_FAST8_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_fast8_t;
#endif
#endif
#endif
/// @def ckd_uint_fast8_t
/// @brief Checked integer type ckd_uint_fast8_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_fast8_t  /* implementation defined type */
#else
#ifdef UINT_FAST8_MAX
#if UINT_FAST8_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_fast8_t;
#elif UINT_FAST8_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_fast8_t;
#elif UINT_FAST8_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_fast8_t;
#elif UINT_FAST8_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_fast8_t;
#elif UINT_FAST8_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_fast8_t;
#endif
#endif
#endif
/// @def ckd_int16_t
/// @brief Checked integer type ckd_int16_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int16_t  /* implementation defined type */
#else
#ifdef INT16_MAX
#if INT16_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int16_t;
#elif INT16_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int16_t;
#elif INT16_MAX == INT_MAX
typedef ckd_int_t ckd_int16_t;
#elif INT16_MAX == LONG_MAX
typedef ckd_long_t ckd_int16_t;
#elif INT16_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int16_t;
#endif
#endif
#endif
/// @def ckd_uint16_t
/// @brief Checked integer type ckd_uint16_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint16_t  /* implementation defined type */
#else
#ifdef UINT16_MAX
#if UINT16_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint16_t;
#elif UINT16_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint16_t;
#elif UINT16_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint16_t;
#elif UINT16_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint16_t;
#elif UINT16_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint16_t;
#endif
#endif
#endif
/// @def ckd_int_least16_t
/// @brief Checked integer type ckd_int_least16_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_least16_t  /* implementation defined type */
#else
#ifdef INT_LEAST16_MAX
#if INT_LEAST16_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_least16_t;
#elif INT_LEAST16_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_least16_t;
#elif INT_LEAST16_MAX == INT_MAX
typedef ckd_int_t ckd_int_least16_t;
#elif INT_LEAST16_MAX == LONG_MAX
typedef ckd_long_t ckd_int_least16_t;
#elif INT_LEAST16_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_least16_t;
#endif
#endif
#endif
/// @def ckd_uint_least16_t
/// @brief Checked integer type ckd_uint_least16_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_least16_t  /* implementation defined type */
#else
#ifdef UINT_LEAST16_MAX
#if UINT_LEAST16_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_least16_t;
#elif UINT_LEAST16_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_least16_t;
#elif UINT_LEAST16_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_least16_t;
#elif UINT_LEAST16_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_least16_t;
#elif UINT_LEAST16_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_least16_t;
#endif
#endif
#endif
/// @def ckd_int_fast16_t
/// @brief Checked integer type ckd_int_fast16_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_fast16_t  /* implementation defined type */
#else
#ifdef INT_FAST16_MAX
#if INT_FAST16_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_fast16_t;
#elif INT_FAST16_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_fast16_t;
#elif INT_FAST16_MAX == INT_MAX
typedef ckd_int_t ckd_int_fast16_t;
#elif INT_FAST16_MAX == LONG_MAX
typedef ckd_long_t ckd_int_fast16_t;
#elif INT_FAST16_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_fast16_t;
#endif
#endif
#endif
/// @def ckd_uint_fast16_t
/// @brief Checked integer type ckd_uint_fast16_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_fast16_t  /* implementation defined type */
#else
#ifdef UINT_FAST16_MAX
#if UINT_FAST16_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_fast16_t;
#elif UINT_FAST16_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_fast16_t;
#elif UINT_FAST16_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_fast16_t;
#elif UINT_FAST16_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_fast16_t;
#elif UINT_FAST16_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_fast16_t;
#endif
#endif
#endif
/// @def ckd_int32_t
/// @brief Checked integer type ckd_int32_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int32_t  /* implementation defined type */
#else
#ifdef INT32_MAX
#if INT32_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int32_t;
#elif INT32_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int32_t;
#elif INT32_MAX == INT_MAX
typedef ckd_int_t ckd_int32_t;
#elif INT32_MAX == LONG_MAX
typedef ckd_long_t ckd_int32_t;
#elif INT32_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int32_t;
#endif
#endif
#endif
/// @def ckd_uint32_t
/// @brief Checked integer type ckd_uint32_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint32_t  /* implementation defined type */
#else
#ifdef UINT32_MAX
#if UINT32_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint32_t;
#elif UINT32_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint32_t;
#elif UINT32_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint32_t;
#elif UINT32_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint32_t;
#elif UINT32_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint32_t;
#endif
#endif
#endif
/// @def ckd_int_least32_t
/// @brief Checked integer type ckd_int_least32_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_least32_t  /* implementation defined type */
#else
#ifdef INT_LEAST32_MAX
#if INT_LEAST32_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_least32_t;
#elif INT_LEAST32_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_least32_t;
#elif INT_LEAST32_MAX == INT_MAX
typedef ckd_int_t ckd_int_least32_t;
#elif INT_LEAST32_MAX == LONG_MAX
typedef ckd_long_t ckd_int_least32_t;
#elif INT_LEAST32_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_least32_t;
#endif
#endif
#endif
/// @def ckd_uint_least32_t
/// @brief Checked integer type ckd_uint_least32_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_least32_t  /* implementation defined type */
#else
#ifdef UINT_LEAST32_MAX
#if UINT_LEAST32_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_least32_t;
#elif UINT_LEAST32_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_least32_t;
#elif UINT_LEAST32_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_least32_t;
#elif UINT_LEAST32_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_least32_t;
#elif UINT_LEAST32_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_least32_t;
#endif
#endif
#endif
/// @def ckd_int_fast32_t
/// @brief Checked integer type ckd_int_fast32_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_fast32_t  /* implementation defined type */
#else
#ifdef INT_FAST32_MAX
#if INT_FAST32_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_fast32_t;
#elif INT_FAST32_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_fast32_t;
#elif INT_FAST32_MAX == INT_MAX
typedef ckd_int_t ckd_int_fast32_t;
#elif INT_FAST32_MAX == LONG_MAX
typedef ckd_long_t ckd_int_fast32_t;
#elif INT_FAST32_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_fast32_t;
#endif
#endif
#endif
/// @def ckd_uint_fast32_t
/// @brief Checked integer type ckd_uint_fast32_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_fast32_t  /* implementation defined type */
#else
#ifdef UINT_FAST32_MAX
#if UINT_FAST32_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_fast32_t;
#elif UINT_FAST32_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_fast32_t;
#elif UINT_FAST32_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_fast32_t;
#elif UINT_FAST32_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_fast32_t;
#elif UINT_FAST32_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_fast32_t;
#endif
#endif
#endif
/// @def ckd_int64_t
/// @brief Checked integer type ckd_int64_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int64_t  /* implementation defined type */
#else
#ifdef INT64_MAX
#if INT64_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int64_t;
#elif INT64_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int64_t;
#elif INT64_MAX == INT_MAX
typedef ckd_int_t ckd_int64_t;
#elif INT64_MAX == LONG_MAX
typedef ckd_long_t ckd_int64_t;
#elif INT64_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int64_t;
#endif
#endif
#endif
/// @def ckd_uint64_t
/// @brief Checked integer type ckd_uint64_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint64_t  /* implementation defined type */
#else
#ifdef UINT64_MAX
#if UINT64_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint64_t;
#elif UINT64_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint64_t;
#elif UINT64_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint64_t;
#elif UINT64_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint64_t;
#elif UINT64_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint64_t;
#endif
#endif
#endif
/// @def ckd_int_least64_t
/// @brief Checked integer type ckd_int_least64_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_least64_t  /* implementation defined type */
#else
#ifdef INT_LEAST64_MAX
#if INT_LEAST64_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_least64_t;
#elif INT_LEAST64_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_least64_t;
#elif INT_LEAST64_MAX == INT_MAX
typedef ckd_int_t ckd_int_least64_t;
#elif INT_LEAST64_MAX == LONG_MAX
typedef ckd_long_t ckd_int_least64_t;
#elif INT_LEAST64_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_least64_t;
#endif
#endif
#endif
/// @def ckd_uint_least64_t
/// @brief Checked integer type ckd_uint_least64_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_least64_t  /* implementation defined type */
#else
#ifdef UINT_LEAST64_MAX
#if UINT_LEAST64_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_least64_t;
#elif UINT_LEAST64_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_least64_t;
#elif UINT_LEAST64_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_least64_t;
#elif UINT_LEAST64_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_least64_t;
#elif UINT_LEAST64_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_least64_t;
#endif
#endif
#endif
/// @def ckd_int_fast64_t
/// @brief Checked integer type ckd_int_fast64_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_int_fast64_t  /* implementation defined type */
#else
#ifdef INT_FAST64_MAX
#if INT_FAST64_MAX == SCHAR_MAX
typedef ckd_schar_t ckd_int_fast64_t;
#elif INT_FAST64_MAX == SHRT_MAX
typedef ckd_shrt_t ckd_int_fast64_t;
#elif INT_FAST64_MAX == INT_MAX
typedef ckd_int_t ckd_int_fast64_t;
#elif INT_FAST64_MAX == LONG_MAX
typedef ckd_long_t ckd_int_fast64_t;
#elif INT_FAST64_MAX == LLONG_MAX
typedef ckd_llong_t ckd_int_fast64_t;
#endif
#endif
#endif
/// @def ckd_uint_fast64_t
/// @brief Checked integer type ckd_uint_fast64_t is an alias to one of ckd_*_t basic types.
#ifdef CKD_DOXYGEN
#define ckd_uint_fast64_t  /* implementation defined type */
#else
#ifdef UINT_FAST64_MAX
#if UINT_FAST64_MAX == UCHAR_MAX
typedef ckd_uchar_t ckd_uint_fast64_t;
#elif UINT_FAST64_MAX == USHRT_MAX
typedef ckd_ushrt_t ckd_uint_fast64_t;
#elif UINT_FAST64_MAX == UINT_MAX
typedef ckd_uint_t ckd_uint_fast64_t;
#elif UINT_FAST64_MAX == ULONG_MAX
typedef ckd_ulong_t ckd_uint_fast64_t;
#elif UINT_FAST64_MAX == ULLONG_MAX
typedef ckd_ullong_t ckd_uint_fast64_t;
#endif
#endif
#endif

// ]]]
// Check integer accessors [[[

/**
 * @def ckd_overflow(ckd_type_t x)
 * @brief If ckd_type is a checked integer type, the ckd_overflow macro indicates if x was computed
using an operation that overflowed or suffered truncation or misinterpretation of sign.
 * @param x One of checked integer types.
 * @return The ckd_overflow macro returns true if overflow, truncation, or misinterpretation of sign
 * occurred when x was computed and false otherwise.
 */
#define ckd_overflow(x)  ((x)._ckd_Overflow)

/**
 * @def ckd_value(ckd_type x)
 * @brief If ckd_type is a checked integer type, the ckd_value macro indicates the value of x.
 * If the overflow flag is set to false, the value correctly represents the mathematical value of whatever
 * operation(s) produced x. Otherwise, the value of x is converted as if by assignment to the type of x.
 * @param x One of checked integer types.
 * @return The ckd_value macro returns the value of x.
 */
#define ckd_value(x)     ((x)._ckd_Value)

// ]]]
// ckd_mk_* functions [[[
/**
 * @brief This function constructs a checked integer type ckd_char_t
 * given an unchecked integer of type char and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_char_t ckd_mk_char_t(char _ckd_value, bool _ckd_overflow) {
	const ckd_char_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_schar_t
 * given an unchecked integer of type signed char and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_schar_t ckd_mk_schar_t(signed char _ckd_value, bool _ckd_overflow) {
	const ckd_schar_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uchar_t
 * given an unchecked integer of type unsigned char and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uchar_t ckd_mk_uchar_t(unsigned char _ckd_value, bool _ckd_overflow) {
	const ckd_uchar_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_shrt_t
 * given an unchecked integer of type short and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_shrt_t ckd_mk_shrt_t(short _ckd_value, bool _ckd_overflow) {
	const ckd_shrt_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_ushrt_t
 * given an unchecked integer of type unsigned short and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_ushrt_t ckd_mk_ushrt_t(unsigned short _ckd_value, bool _ckd_overflow) {
	const ckd_ushrt_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_t
 * given an unchecked integer of type int and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_t ckd_mk_int_t(int _ckd_value, bool _ckd_overflow) {
	const ckd_int_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_t
 * given an unchecked integer of type unsigned int and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_t ckd_mk_uint_t(unsigned int _ckd_value, bool _ckd_overflow) {
	const ckd_uint_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_long_t
 * given an unchecked integer of type long and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_long_t ckd_mk_long_t(long _ckd_value, bool _ckd_overflow) {
	const ckd_long_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_ulong_t
 * given an unchecked integer of type unsigned long and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_ulong_t ckd_mk_ulong_t(unsigned long _ckd_value, bool _ckd_overflow) {
	const ckd_ulong_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_llong_t
 * given an unchecked integer of type long long and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_llong_t ckd_mk_llong_t(long long _ckd_value, bool _ckd_overflow) {
	const ckd_llong_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_ullong_t
 * given an unchecked integer of type unsigned long long and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_ullong_t ckd_mk_ullong_t(unsigned long long _ckd_value, bool _ckd_overflow) {
	const ckd_ullong_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int128_t
 * given an unchecked integer of type __int128 and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int128_t ckd_mk_int128_t(__int128 _ckd_value, bool _ckd_overflow) {
	const ckd_int128_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint128_t
 * given an unchecked integer of type unsigned __int128 and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint128_t ckd_mk_uint128_t(unsigned __int128 _ckd_value, bool _ckd_overflow) {
	const ckd_uint128_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_intmax_t
 * given an unchecked integer of type intmax_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_intmax_t ckd_mk_intmax_t(intmax_t _ckd_value, bool _ckd_overflow) {
	const ckd_intmax_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uintmax_t
 * given an unchecked integer of type uintmax_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uintmax_t ckd_mk_uintmax_t(uintmax_t _ckd_value, bool _ckd_overflow) {
	const ckd_uintmax_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_size_t
 * given an unchecked integer of type size_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_size_t ckd_mk_size_t(size_t _ckd_value, bool _ckd_overflow) {
	const ckd_size_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_ptrdiff_t
 * given an unchecked integer of type ptrdiff_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_ptrdiff_t ckd_mk_ptrdiff_t(ptrdiff_t _ckd_value, bool _ckd_overflow) {
	const ckd_ptrdiff_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_intptr_t
 * given an unchecked integer of type intptr_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_intptr_t ckd_mk_intptr_t(intptr_t _ckd_value, bool _ckd_overflow) {
	const ckd_intptr_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uintptr_t
 * given an unchecked integer of type uintptr_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uintptr_t ckd_mk_uintptr_t(uintptr_t _ckd_value, bool _ckd_overflow) {
	const ckd_uintptr_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int8_t
 * given an unchecked integer of type int8_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int8_t ckd_mk_int8_t(int8_t _ckd_value, bool _ckd_overflow) {
	const ckd_int8_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint8_t
 * given an unchecked integer of type uint8_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint8_t ckd_mk_uint8_t(uint8_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint8_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_least8_t
 * given an unchecked integer of type int_least8_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_least8_t ckd_mk_int_least8_t(int_least8_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_least8_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_least8_t
 * given an unchecked integer of type uint_least8_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_least8_t ckd_mk_uint_least8_t(uint_least8_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_least8_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_fast8_t
 * given an unchecked integer of type int_fast8_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_fast8_t ckd_mk_int_fast8_t(int_fast8_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_fast8_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_fast8_t
 * given an unchecked integer of type uint_fast8_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_fast8_t ckd_mk_uint_fast8_t(uint_fast8_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_fast8_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int16_t
 * given an unchecked integer of type int16_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int16_t ckd_mk_int16_t(int16_t _ckd_value, bool _ckd_overflow) {
	const ckd_int16_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint16_t
 * given an unchecked integer of type uint16_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint16_t ckd_mk_uint16_t(uint16_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint16_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_least16_t
 * given an unchecked integer of type int_least16_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_least16_t ckd_mk_int_least16_t(int_least16_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_least16_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_least16_t
 * given an unchecked integer of type uint_least16_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_least16_t ckd_mk_uint_least16_t(uint_least16_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_least16_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_fast16_t
 * given an unchecked integer of type int_fast16_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_fast16_t ckd_mk_int_fast16_t(int_fast16_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_fast16_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_fast16_t
 * given an unchecked integer of type uint_fast16_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_fast16_t ckd_mk_uint_fast16_t(uint_fast16_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_fast16_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int32_t
 * given an unchecked integer of type int32_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int32_t ckd_mk_int32_t(int32_t _ckd_value, bool _ckd_overflow) {
	const ckd_int32_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint32_t
 * given an unchecked integer of type uint32_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint32_t ckd_mk_uint32_t(uint32_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint32_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_least32_t
 * given an unchecked integer of type int_least32_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_least32_t ckd_mk_int_least32_t(int_least32_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_least32_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_least32_t
 * given an unchecked integer of type uint_least32_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_least32_t ckd_mk_uint_least32_t(uint_least32_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_least32_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_fast32_t
 * given an unchecked integer of type int_fast32_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_fast32_t ckd_mk_int_fast32_t(int_fast32_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_fast32_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_fast32_t
 * given an unchecked integer of type uint_fast32_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_fast32_t ckd_mk_uint_fast32_t(uint_fast32_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_fast32_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int64_t
 * given an unchecked integer of type int64_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int64_t ckd_mk_int64_t(int64_t _ckd_value, bool _ckd_overflow) {
	const ckd_int64_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint64_t
 * given an unchecked integer of type uint64_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint64_t ckd_mk_uint64_t(uint64_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint64_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_least64_t
 * given an unchecked integer of type int_least64_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_least64_t ckd_mk_int_least64_t(int_least64_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_least64_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_least64_t
 * given an unchecked integer of type uint_least64_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_least64_t ckd_mk_uint_least64_t(uint_least64_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_least64_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_int_fast64_t
 * given an unchecked integer of type int_fast64_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_int_fast64_t ckd_mk_int_fast64_t(int_fast64_t _ckd_value, bool _ckd_overflow) {
	const ckd_int_fast64_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}
/**
 * @brief This function constructs a checked integer type ckd_uint_fast64_t
 * given an unchecked integer of type uint_fast64_t and an overflow flag.
 * If the overflow flag is true, the value is assumed to have involved overflow,
 * truncation, or  misinterpretation of sign.* Otherwise the value is assumed to
 * be mathematically correct.
 * @param _ckd_value Unchecked integer.
 * @param _ckd_overflow Overflow flag.
 * @return Return a checked type that represents the value indicated by value and the exact
 * state indicated by overflow.
 */
_ckd_fconst ckd_uint_fast64_t ckd_mk_uint_fast64_t(uint_fast64_t _ckd_value, bool _ckd_overflow) {
	const ckd_uint_fast64_t _ckd_tmp = {_ckd_value, _ckd_overflow};
	return _ckd_tmp;
}

// ]]]
// Macro helpers [[[
// ]]]
// Generic macros implementation [[[

// These two should provide _ckd_$OP_3 and _ckd_$OP_2
#if __GNUC__ >= 5 && !CKD_NOGNU_SOURCE
#include "ckdint/ckdint_gnu.h"
#else
#include "ckdint/ckdint_nognu.h"
#endif

/// @def ckd_mk(value, overflow)
/// @param value unchecked integer
/// @param overflow inexact flag
/// @brief This macro constructs a checked integer type given an unchecked integer and an inexact flag.
/// If the inexact flag is true, the value is assumed to have involved overflow, truncation, or
/// misinterpretation of sign.* Otherwise the value is assumed to be mathematically correct.
/// Constructing a checked integer with an inexact flag set to true can be useful when
/// explicitly indicating an error inside an expression.
/// @return This macro returns a checked type that represents the value indicated by value and the exact state
/// indicated by inexact.
#ifdef CKDINT_GNU
#define ckd_mk(value, overflow)  _ckd_gnu_mk(value, overflow)
#elif defined(CKDINT_NOGNU)
#define ckd_mk(value, overflow)  _ckd_nognu_mk(value, overflow)
#else
#error "ckdint internal error - neight gnu or nognu version was included"
#endif


/// @def _ckd_add_N
/// @brief Macro overload on number of arguments for ckd_add
/// @see ckd_add
#ifdef CKDINT_GNU
#define _ckd_add_N(_1, _2, _3, N, ...)  _ckd_gnu_add_##N(_1, _2, _3)
#elif defined(CKDINT_NOGNU)
#define _ckd_add_N(_1, _2, _3, N, ...)  _ckd_nognu_add_##N(_1, _2, _3)
#else
#error "ckdint internal error - neight gnu or nognu version was included"
#endif

/**
 * @def ckd_add(result, a, b)
 * @brief `bool ckd_add(type1 *result, type2 a, type3 b);` or `ckd_type_t ckd_add(type1 a, type2 b);`
 * @param a Any checked or unchecked integer type other than plain char, bool, or an enumeration type.
 * @param b Any checked or unchecked integer type other than plain char, bool, or an enumeration type.
 * @param result Modifiable lvalue of any integer type.
 * @return In the first-form return false if type1 is sufficient to hold the mathematical result of
 * the computation, which is then stored in *result... Otherwise, these macros return true.. In this
 * case, *result is the mathematical result reduced by modular arithmetic on two’s-complement
 * representation with silent wrap-around on overflow. If *result is a checked integer type then its
 * overflow flag will equal the macro’s return value.
 * The macros of the second-form return a checked integer type that indicates the result of the
 * computation as well as an overflow flag.
 *
 * These macros perform operation add of the mathematical values of a and b.
 * In the first form, they store the result of the operation in *result, and in the second form, they return
 * the result as a checked integer.  Each operation is performed as if both operands were represented in a
 * signed integer type with infinite range, and the result was then converted from this integer type to a
 * particular type.  For the first form, this particular type is type1. For the second form, this type is the
 * type that would have been used had the operands undergone usual arithmetic conversion. (Section 6.3.1.8)
 */
#ifdef CKD_DOXYGEN
#define ckd_add(result, a, b)
#else
#define ckd_add(...)  _ckd_add_N(__VA_ARGS__, 3, 2)
#endif


/// @def _ckd_sub_N
/// @brief Macro overload on number of arguments for ckd_sub
/// @see ckd_sub
#ifdef CKDINT_GNU
#define _ckd_sub_N(_1, _2, _3, N, ...)  _ckd_gnu_sub_##N(_1, _2, _3)
#elif defined(CKDINT_NOGNU)
#define _ckd_sub_N(_1, _2, _3, N, ...)  _ckd_nognu_sub_##N(_1, _2, _3)
#else
#error "ckdint internal error - neight gnu or nognu version was included"
#endif

/**
 * @def ckd_sub(result, a, b)
 * @brief `bool ckd_sub(type1 *result, type2 a, type3 b);` or `ckd_type_t ckd_sub(type1 a, type2 b);`
 * @param a Any checked or unchecked integer type other than plain char, bool, or an enumeration type.
 * @param b Any checked or unchecked integer type other than plain char, bool, or an enumeration type.
 * @param result Modifiable lvalue of any integer type.
 * @return In the first-form return false if type1 is sufficient to hold the mathematical result of
 * the computation, which is then stored in *result... Otherwise, these macros return true.. In this
 * case, *result is the mathematical result reduced by modular arithmetic on two’s-complement
 * representation with silent wrap-around on overflow. If *result is a checked integer type then its
 * overflow flag will equal the macro’s return value.
 * The macros of the second-form return a checked integer type that indicates the result of the
 * computation as well as an overflow flag.
 *
 * These macros perform operation sub of the mathematical values of a and b.
 * In the first form, they store the result of the operation in *result, and in the second form, they return
 * the result as a checked integer.  Each operation is performed as if both operands were represented in a
 * signed integer type with infinite range, and the result was then converted from this integer type to a
 * particular type.  For the first form, this particular type is type1. For the second form, this type is the
 * type that would have been used had the operands undergone usual arithmetic conversion. (Section 6.3.1.8)
 */
#ifdef CKD_DOXYGEN
#define ckd_sub(result, a, b)
#else
#define ckd_sub(...)  _ckd_sub_N(__VA_ARGS__, 3, 2)
#endif


/// @def _ckd_mul_N
/// @brief Macro overload on number of arguments for ckd_mul
/// @see ckd_mul
#ifdef CKDINT_GNU
#define _ckd_mul_N(_1, _2, _3, N, ...)  _ckd_gnu_mul_##N(_1, _2, _3)
#elif defined(CKDINT_NOGNU)
#define _ckd_mul_N(_1, _2, _3, N, ...)  _ckd_nognu_mul_##N(_1, _2, _3)
#else
#error "ckdint internal error - neight gnu or nognu version was included"
#endif

/**
 * @def ckd_mul(result, a, b)
 * @brief `bool ckd_mul(type1 *result, type2 a, type3 b);` or `ckd_type_t ckd_mul(type1 a, type2 b);`
 * @param a Any checked or unchecked integer type other than plain char, bool, or an enumeration type.
 * @param b Any checked or unchecked integer type other than plain char, bool, or an enumeration type.
 * @param result Modifiable lvalue of any integer type.
 * @return In the first-form return false if type1 is sufficient to hold the mathematical result of
 * the computation, which is then stored in *result... Otherwise, these macros return true.. In this
 * case, *result is the mathematical result reduced by modular arithmetic on two’s-complement
 * representation with silent wrap-around on overflow. If *result is a checked integer type then its
 * overflow flag will equal the macro’s return value.
 * The macros of the second-form return a checked integer type that indicates the result of the
 * computation as well as an overflow flag.
 *
 * These macros perform operation mul of the mathematical values of a and b.
 * In the first form, they store the result of the operation in *result, and in the second form, they return
 * the result as a checked integer.  Each operation is performed as if both operands were represented in a
 * signed integer type with infinite range, and the result was then converted from this integer type to a
 * particular type.  For the first form, this particular type is type1. For the second form, this type is the
 * type that would have been used had the operands undergone usual arithmetic conversion. (Section 6.3.1.8)
 */
#ifdef CKD_DOXYGEN
#define ckd_mul(result, a, b)
#else
#define ckd_mul(...)  _ckd_mul_N(__VA_ARGS__, 3, 2)
#endif

// ]]]
// Extensions [[[

/// @def _ckd_inc_in
/// @brief internal callback from ckd_inc
#ifdef CKDINT_GNU
#define _ckd_inc_in(pnt, inc, ...)  __extension__({ \
		__auto_type _ckd_pnt = (pnt); \
		ckd_add(_ckd_pnt, *_ckd_pnt, (inc)); \
		})
#else
#define _ckd_inc_in(pnt, inc, ...)  ckd_add((pnt), *(pnt), (inc))
#endif

/**
 * @def ckd_inc(...)
 * @brief ckd_inc(pnt) or ckd_inc(pnt, inc).
 * Increments value pointed to by pnt by 1 or by inc.
 * @param pnt Pointer to checked integer type or normal integer type.
 * Warning: it is evaulated twice in nognu mode!
 * @param inc The value to increment with. 1 by default.
 * @return overflow
 */
#ifdef CKD_DOXYGEN
#define ckd_inc(pnt, inc)
#else
#define ckd_inc(...)  _ckd_inc_in(__VA_ARGS__, 1)
#endif

/// @def _ckd_dec_in
/// @brief internal callback from ckd_dec
#ifdef CKDINT_GNU
#define _ckd_dec_in(pnt, inc, ...)  __extension__({ \
		__auto_type _ckd_pnt = (pnt); \
		ckd_sub(_ckd_pnt, *_ckd_pnt, (inc)); \
		})
#else
#define _ckd_dec_in(pnt, inc, ...)  ckd_sub((pnt), *(pnt), (inc))
#endif

/**
 * @def ckd_dec(...)
 * @brief ckd_dec(pnt) or ckd_dec(pnt, inc).
 * Decrements value pointed to by pnt by 1 or by inc.
 * @param pnt Pointer to checked integer type or normal integer type.
 * Warning: it is evaulated twice in nognu mode!
 * @param inc The value to increment with. 1 by default.
 * @return overflow
 */
#ifdef CKD_DOXYGEN
#define ckd_dec(pnt, inc)
#else
#define ckd_dec(...)  _ckd_dec_in(__VA_ARGS__, 1)
#endif

// ]]]
// EOF [[[
#endif  // CKDINT_H_
// ]]]
// vim: ft=c

