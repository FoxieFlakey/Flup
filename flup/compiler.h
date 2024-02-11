#ifndef header_1705122589_b296386d_ce1b_480a_afd9_413602d9b57c_compiler_h
#define header_1705122589_b296386d_ce1b_480a_afd9_413602d9b57c_compiler_h

/**
 * @file
 * @brief Compiler's feature detection stuffs.
 *
 * This file macros define a macro which expands
 * to 1 if available or (0 or undefined) if not.
 *
 * @warning Do not trust the value shown in generated
 *   docs the value depends on the compiler settings
 *   therefore don't rely value shown in Doxygen output
 */

/**
 * @brief Apple Blocks syntax
 *
 * Apple blocks syntax can be enabled with
 * `-fblocks` on Clang.
 * See <a href="https://clang.llvm.org/docs/Block-ABI-Apple.html">Clang</a>
 */
#define FLUP_APPLE_BLOCKS_AVAILABLE 0

#if defined(__clang__) && __has_extension(blocks)
#undef FLUP_APPLE_BLOCKS_AVAILABLE
# define FLUP_APPLE_BLOCKS_AVAILABLE 1
#endif

#endif

