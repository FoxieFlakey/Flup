#include <limits.h>
#include <sched.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "flup/core/panic.h"
#include "flup/attributes.h"
#include "flup/core/logger.h"

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

[[noreturn]]
static void hardPanic(const char* format, va_list list) {
  fprintf(stderr, "[HARD PANIC] Hard panic occured OwO: ");
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
  vfprintf(stderr, format, list);
#ifdef __clang__
# pragma clang diagnostic pop
#endif
  fprintf(stderr, "\n");
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
  flup_pr_fatal("Panic occured >w<: %s", panicBuffer);
  flup_pr_info("Flushing logs");
  flup_flush_logs(NULL);
  abort();
}


