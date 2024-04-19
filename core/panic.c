#include <limits.h>
#include <sched.h>
#include <stdarg.h>
#include <errno.h>
#include <stdatomic.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

#include "flup/core/panic.h"
#include "flup/attributes.h"
#include "flup/core/logger.h"
#include "flup/stacktrace/stacktrace.h"

/*
1 << 10, 1 KiB log buffer
1 << 11, 2 KiB log buffer
1 << 12, 4 KiB log buffer
*/
#define PANIC_BUFFER_SIZE (1 << 11)

[[noreturn]]
FLUP_PUBLIC
void flup_panic(const char* format, ...) {
  va_list list;
  va_start(list, format);
  flup_vpanic(format, list);
  va_end(list);
}

static const char* snip1 = "--------[ stacktrace UwU ]--------";
static const char* snip2 = "----------------------------------";

static void dumpStacktrace(void (^printMsg)(const char* fmt, ...)) {
  int ret = flup_stacktrace_walk_current_block(^bool (const flup_stacktrace_element* element) {
    char ipHexedBuffer[64];
    const char* symName = NULL; 
    if (element->symbol)
      symName = element->symbol->symbolName;
    
    if (!symName) {
      snprintf(ipHexedBuffer, sizeof(ipHexedBuffer), "0x%" PRIxPTR "+0x%04" PRIxPTR, element->ip, element->ipOffset);
      symName = ipHexedBuffer;
    }
    
    if (element->source)
      printMsg("  at %s(%s:%d:%d)", symName, element->source->file, element->source->line, element->source->column);
    else if (element->symbol)
      printMsg("  at %s(Source.c:-1:-1)", symName);
    if (element->count > 1)
      printMsg("  ... previous frame repeats %d times ...", element->count - 1);
    return true;
  });
  if (ret == -ENOSYS)
    printMsg("Stacktrace unavailable: %d", ret);
}

static thread_local bool currentThreadInHardPanic = false;
static atomic_bool programInHardPanic = false;

[[noreturn]]
static void hardPanic(const char* format, va_list list) {
  currentThreadInHardPanic = true;
  
  if (atomic_exchange(&programInHardPanic, true) == true) {
    // Nothing we can do if double hard panic occured on same thread
    if (currentThreadInHardPanic == true) {
      // Exit in most abrupt way ever
      _exit(EXIT_FAILURE);
    }
    
    // Idk, wait forever?
    while (1) {
      sched_yield();
      sleep(UINT_MAX);
    }
  }
  
  fprintf(stderr, "[HARD PANIC] %s", snip1);
  fprintf(stderr, "[HARD PANIC] Hard panic occured OwO: ");
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
  vfprintf(stderr, format, list);
  fprintf(stderr, "\n");
  dumpStacktrace(^void (const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);
    va_end(args);
  });
#ifdef __clang__
# pragma clang diagnostic pop
#endif
  fprintf(stderr, "[HARD PANIC] %s", snip2);
  fprintf(stderr, "[HARD PANIC] Logs not flushed!\n");
  abort();
}

[[noreturn]]
FLUP_PUBLIC
void flup_vpanic(const char* format, va_list list) {
  flup_is_current_thread_aborting = true;
  
  // Is an program abortion is in progress
  if (atomic_exchange(&flup_is_aborting, true) == true) {
    // Current thread attempt to abort twice just
    // do hard panic to break the loop
    if (flup_is_current_thread_aborting)
      hardPanic(format, list);
    
    // Abortion happening on other thread while another
    // one in progress just sleep forever and yield to
    // let other thread run
    while (1) {
      sched_yield();
      sleep(UINT_MAX);
    }
  }
  
  static char panicBuffer[PANIC_BUFFER_SIZE];
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
  // Foxie don't care if its truncated
  (void) vsnprintf(panicBuffer, sizeof(panicBuffer), format, list);
#ifdef __clang__
# pragma clang diagnostic pop
#endif
  
  flup_pr_fatal("%s", snip1);
  flup_pr_fatal("Panic occured >w<: %s", panicBuffer);
  dumpStacktrace(^(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    flup_vprintk(FLUP_FATAL, fmt, args);
    va_end(args);
  });
  flup_pr_fatal("%s", snip2);
  flup_pr_info("Flushing logs");
  flup_flush_logs(NULL);
  abort();
}


