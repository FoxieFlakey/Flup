#ifndef header_1706007321_a724a59c_4fab_4bf8_afd3_2f00c5df4a22_typecheck_h
#define header_1706007321_a724a59c_4fab_4bf8_afd3_2f00c5df4a22_typecheck_h

/**
 * @file
 * @brief Type check macros for macros
 *
 * Contain helper macros for checking
 * types in a macro.
 */

/**
 * @brief Check type for @a var's type.
 * 
 * Does not take expression only take
 * variable name.
 *
 * @param var The variable to check
 * @param ... Type to check for
 *
 * @return @a var as @a ... type
 */
#define FLUP_CHECK_VAR_TYPE(var, ...) \
  (*((struct {__VA_ARGS__* __flup_check_ptr;}) \
     {&(var)}).__flup_check_ptr)

/**
 * @brief Check function type for @a func's type
 *
 * Does not take expression only take name.
 *
 * @param func Name/variable of function to check for type
 * @param retType Return value of the function
 * @param ... List of arguments types
 *
 * @return @a func as function with correct args
 *   and return value
 */
#define FLUP_CHECK_VAR_FUNC_TYPE(func, retType, ...) \
  (*((struct {retType (*__flup_check_ptr)(__VA_ARGS__);}) \
     {&(func)}).__flup_check_ptr)

/**
 * @brief Check function type for @a block's type
 *
 * Does not take expression only take name.
 *
 * @param block Variable of block to check for type
 * @param retType Return value of the block
 * @param ... List of arguments types as vararg
 *
 * @return @a block as blocks with correct args
 *   and return values
 */
#define FLUP_CHECK_VAR_BLOCK_TYPE(block, retType, ...) \
  (*((struct {retType (^*__flup_check_ptr)(__VA_ARGS__);}) \
     {&(block)}).__flup_check_ptr)

#endif

