#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "flup/core/logger.h"
#include "flup/attributes.h"
#include "flup/concurrency/cond.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/buffer/circular_buffer.h"

/*
1 << 12, 4 KiB log buffer
1 << 13, 8 KiB log buffer
1 << 14, 16 KiB log buffer
1 << 15, 32 KiB log buffer
1 << 16, 64 KiB log buffer
*/
#define BUFFER_SIZE (1 << 16)

/*
1 << 11, 2 KiB log buffer
1 << 12, 4 KiB log buffer
1 << 13, 8 KiB log buffer
*/
#define THREAD_BUFFER_SIZE (1 << 12)

FLUP_PUBLIC_VAR
char flup_logbuffer[BUFFER_SIZE];

/// The size of logbuffer
FLUP_PUBLIC_VAR
size_t flup_logbuffer_size = BUFFER_SIZE;

/// Is an abort in progress (panic, bug, etc)
FLUP_PUBLIC_VAR
atomic_bool flup_is_in_abort = false;

FLUP_CIRCULAR_BUFFER_DEFINE_STATIC(logBufferStruct, flup_logbuffer, BUFFER_SIZE);
FLUP_MUTEX_DEFINE_STATIC(loggerLock);
FLUP_COND_DEFINE_STATIC(loggerCond);

FLUP_PUBLIC
void flup__printk(const flup_printk_call_site_info* callSite, flup_loglevel loglevel, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flup__vprintk(callSite, loglevel, fmt, args);
  va_end(args);
}

static const char* loglevelToStr(flup_loglevel loglevel) {
  switch (loglevel) {
    case FLUP_FATAL:
      return "FATAL";
    case FLUP_ALERT:
      return "ALERT";
    case FLUP_CRITICAL:
      return "CRITICAL";
    case FLUP_ERROR:
      return "ERROR";
    case FLUP_WARN:
      return "WARN";
    case FLUP_NOTICE:
      return "NOTICE";
    case FLUP_INFO:
      return "INFO";
    case FLUP_VERBOSE:
      return "VERBOSE";
    case FLUP_DEBUG:
      return "DEBUG";
  }
  return NULL;
}

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif

FLUP_PUBLIC
void flup__vprintk(const flup_printk_call_site_info* callSite, flup_loglevel loglevel, const char* fmt, va_list args) {
  static thread_local char threadBuffer[THREAD_BUFFER_SIZE];
  
  size_t sizeLeft = sizeof(threadBuffer);
  char* currentPointer = threadBuffer;
  *currentPointer = '\0';
  
  int writtenChars = vsnprintf(currentPointer, sizeLeft, fmt, args);
  if ((size_t) writtenChars + 1 > sizeLeft)
    goto overflow_occured;
  currentPointer += writtenChars;
  
overflow_occured:;
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  printf("Line: [%ju] [<placeholder>/%s] [%s:%d#%s] %s\n", (uintmax_t) time.tv_sec, loglevelToStr(loglevel), callSite->file, callSite->line, callSite->shortFuncName, threadBuffer);
}

#ifdef __clang__
#pragma GCC diagnostic pop
#endif


