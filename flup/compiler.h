#ifndef header_1705122589_b296386d_ce1b_480a_afd9_413602d9b57c_compiler_h
#define header_1705122589_b296386d_ce1b_480a_afd9_413602d9b57c_compiler_h

// Check for compiler features
// and also for compiler builtins

#if !defined(FLUP_APPLE_BLOCKS_AVAILABLE) && defined(__clang__) && __has_extension(blocks)
# define FLUP_APPLE_BLOCKS_AVAILABLE
#endif

#endif

