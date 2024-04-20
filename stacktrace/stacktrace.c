#include <stddef.h>
#include <libunwind.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "flup/stacktrace/stacktrace.h"
#include "flup/attributes.h"
#include "common.h"

#ifdef UNW_WORD_MAX
static_assert(UNW_WORD_MAX <= UINTPTR_MAX, "unw_word_t must be smaller than uintptr_t (or can be represented by uintptr_t)");
#else  
static_assert(sizeof(unw_word_t) <= sizeof(uintptr_t), "unw_word_t must be smaller than uintptr_t (or can be represented by uintptr_t)");
#endif

FLUP_PUBLIC
int flup_stacktrace_walk_current(flup_stacktrace_walker_func walker, void* udata) {
  unw_context_t context;
  unw_cursor_t cursor;
  
  unw_getcontext(&context);
  unw_init_local(&cursor, &context);
  
  int stepRet;
  while ((stepRet = unw_step(&cursor))) {
    if (stepRet < 0)
      break;
    
    unw_word_t ip;
    unw_proc_info_t info;
    
    unw_word_t offsetFromSymbol;
    
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    if (unw_get_proc_info(&cursor, &info) == 0)
      offsetFromSymbol = ip - info.start_ip;
    else
      offsetFromSymbol = 0;
    
    flup_stacktrace_symbol sym;
    flup_stacktrace_object object;
    flup_stacktrace_source src;
    
    flup_stacktrace_element element = {
      .ipOffset = (uintptr_t) offsetFromSymbol,
      .ip = (uintptr_t) ip,
      .symbol = NULL,
      .source = NULL,
      .count = 0
    };
    
    char symbolNameBuffer[256];
    if (stacktrace_get_symbol((uintptr_t) ip, &sym) == 0)
      element.symbol = &sym;
    
    if (!element.symbol) {
      unw_word_t offset;
      // If Flup's method failed, try use what libunwinder gives
      if (unw_get_proc_name(&cursor, symbolNameBuffer, sizeof(symbolNameBuffer), &offset) == 0) {
        sym.symbolName = symbolNameBuffer;
        sym.symbolAddr = (uintptr_t) (ip - offset);
        element.ipOffset = (uintptr_t) offset;
        
        element.symbol = &sym;
      }
    }
    
    if (stacktrace_get_object((uintptr_t) ip, &object) == 0)
      element.object = &object;
    if (stacktrace_get_source((uintptr_t) ip, &src) == 0)
      element.source = &src;
    
    walker(&element, udata);
  }
  
  return 0;
}

static bool walkerTrampoline(const flup_stacktrace_element* element, void* walker) {
  return ((flup_stacktrace_walker_block) walker)(element);
}

FLUP_PUBLIC
int flup_stacktrace_walk_current_block(flup_stacktrace_walker_block walker) {
  return flup_stacktrace_walk_current(walkerTrampoline, walker);
}


