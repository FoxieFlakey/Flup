#ifndef UWU_A2571FD3_D9CA_4DCC_AE85_0CBE6BEC7073_UWU
#define UWU_A2571FD3_D9CA_4DCC_AE85_0CBE6BEC7073_UWU

#include <stdint.h>

#include "flup/attributes.h"
#include "flup/compiler.h"

/**
 * @file
 * @brief Stacktracing functionality
 */

/**
 * @brief Source information
 */
typedef struct flup_stacktrace_source {
  /// The function name
  const char* funcName;
  /// The filename
  const char* file;
  /// The line in file
  int line;
  /// The column in file
  int column;
} flup_stacktrace_source;

/**
 * @brief The shared/static/etc object of origin element
 */
typedef struct flup_stacktrace_object {
  /// Path to the object (isn't always valid path)
  const char* objectPath;
  /// Base address of object
  uintptr_t objectBaseAddress;
} flup_stacktrace_object;

/**
 * @brief Symbol of the origin element
 */
typedef struct flup_stacktrace_symbol {
  /// Name of symbol (may not always resolveable by dlsym)
  const char* symbolName;
  /// Address of symbol
  uintptr_t symbolAddr;
} flup_stacktrace_symbol;

/**
 * @brief Stacktrace element
 */
typedef struct flup_stacktrace_element {
  /// Source info from debuginfo or NULL if unavailable
  const struct flup_stacktrace_source* source;
  /// Symbol of origin NULL if unavailable
  const struct flup_stacktrace_symbol* symbol;
  /// Object of origin NULL if unavailable
  const flup_stacktrace_object* object;
  
  /// Un-offseted instruction pointer
  uintptr_t ip;
  /// Offset to ip relative to the starting ip
  /// of the function or UINTPTR_MAX if unavailable
  uintptr_t ipOffset;
  
  /// Number of same frame occuring
  int count;
} flup_stacktrace_element;

/**
 * @brief A walker function to iterate stacktrace element
 *
 * @note The @p element only valid during this function execution
 *
 * @param element Read-only pointer to stacktrace element
 * @param udata User provided state
 *
 * @return true to continue walking or false if to quit
 */
typedef bool (*flup_stacktrace_walker_func)(const flup_stacktrace_element* element, void* udata);

/**
 * @brief Walk current stack with function
 *
 * This function call @p walker with element of current stack element and
 * udata passed from @p udata
 *
 * @param walker The walker function
 * @param udata The opaque user data to be passed to @p walker
 *
 * @throws -ENOSYS: Stacktrace is not supported
 * @throws -errno: Additional error may be returned but its unspecified
 *
 * @return 0 on success (walker break early still count as success), -errno
 *   on failure (see above)
 */
FLUP_PUBLIC
int flup_stacktrace_walk_current(flup_stacktrace_walker_func walker, void* udata);

#if defined(FLUP_APPLE_BLOCKS_AVAILABLE) && FLUP_APPLE_BLOCKS_AVAILABLE
/**
 * @brief A walker block to iterate stacktrace element
 *
 * @note The @p element only valid during this function execution
 *
 * @param element Read-only pointer to stacktrace element
 *
 * @return true to continue walking or false if to quit
 */
typedef bool (^flup_stacktrace_walker_block)(const flup_stacktrace_element* element);

/**
 * @brief Walk current stack with block
 *
 * Same as @ref flup_stacktrace_walk_current but takes Apple Blocks instead
 *
 * @param walker The walker block
 *
 * @throws -ENOSYS: Stacktrace is not supported
 * @throws -errno: Additional error may be returned but its unspecified
 *
 * @return 0 on success (walker break early still count as success), -errno
 *   on failure (see above)
 */
FLUP_PUBLIC
int flup_stacktrace_walk_current_block(flup_stacktrace_walker_block walker);
#endif

#endif
