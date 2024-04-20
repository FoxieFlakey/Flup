#ifndef UWU_A2571FD3_D9CA_4DCC_AE85_0CBE6BEC7073_UWU
#define UWU_A2571FD3_D9CA_4DCC_AE85_0CBE6BEC7073_UWU

#include <stdint.h>

#include "flup/attributes.h"
#include "flup/compiler.h"

typedef struct flup_stacktrace_source {
  const char* funcName;
  const char* file;
  int line;
  int column;
} flup_stacktrace_source;

typedef struct flup_stacktrace_object {
  const char* objectPath;
  uintptr_t objectBaseAddress;
} flup_stacktrace_object;

typedef struct flup_stacktrace_symbol {
  const char* symbolName;
  uintptr_t symbolAddr;
} flup_stacktrace_symbol;

typedef struct flup_stacktrace_element {
  const struct flup_stacktrace_source* source;
  const struct flup_stacktrace_symbol* symbol;
  const flup_stacktrace_object* object;
  
  uintptr_t ip;
  uintptr_t ipOffset;
  
  // Number of same frame occuring
  int count;
} flup_stacktrace_element;

typedef bool (*flup_stacktrace_walker_func)(const flup_stacktrace_element* element, void* udata);

FLUP_PUBLIC
int flup_stacktrace_walk_current(flup_stacktrace_walker_func walker, void* udata);

#if defined(FLUP_APPLE_BLOCKS_AVAILABLE) && FLUP_APPLE_BLOCKS_AVAILABLE
typedef bool (^flup_stacktrace_walker_block)(const flup_stacktrace_element* element);

FLUP_PUBLIC
int flup_stacktrace_walk_current_block(flup_stacktrace_walker_block walker);
#endif

#endif
