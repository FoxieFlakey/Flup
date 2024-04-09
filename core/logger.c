#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <threads.h>

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
FLUP_PUBLIC_VAR
size_t flup_logbuffer_size = BUFFER_SIZE;
FLUP_PUBLIC_VAR
atomic_bool flup_is_in_abort = false;
FLUP_PUBLIC_VAR
thread_local bool flup_is_thread_aborting = false;

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

FLUP_PUBLIC
void flup__vprintk(const flup_printk_call_site_info* callSite, flup_loglevel loglevel, const char* fmt, va_list args) {
  static thread_local char threadBuffer[THREAD_BUFFER_SIZE];
  flup_log_record record = {
    .logLevel = loglevel,
    .funcPtr = callSite->funcPtr,
    .line = callSite->line,
    .uShortFuncNameOffset = -1,
    .uSourcePathOffset = -1,
    .uMessageOffset = -1
  };
  
  size_t sizeLeft = sizeof(threadBuffer);
  char* currentPointer = threadBuffer;
  int writtenBytes = 0;
  *currentPointer = '\0';
  
  // Append the source path if given
  if (!callSite->sourceFile)
    goto sourceNotGiven;
  
  writtenBytes = snprintf(currentPointer, sizeLeft, "%s", callSite->sourceFile) + 1;
  if ((size_t) writtenBytes > sizeLeft)
    goto overflow_occured;
  // Get offset by subtracting originating array and the
  // pointer of its element
  record.uSourcePathOffset = currentPointer - threadBuffer;
  currentPointer += writtenBytes;
  sizeLeft -= (size_t) writtenBytes;
sourceNotGiven:
  
  // Append the func name if given
  if (!callSite->shortFuncName)
    goto funcNameNotGiven;
  
  writtenBytes = snprintf(currentPointer, sizeLeft, "%s", callSite->shortFuncName) + 1;
  if ((size_t) writtenBytes > sizeLeft)
    goto overflow_occured;
  // Get offset by subtracting originating array and the
  // pointer of its element
  record.uShortFuncNameOffset = currentPointer - threadBuffer;
  currentPointer += writtenBytes;
  sizeLeft -= (size_t) writtenBytes;
funcNameNotGiven:
  
  // Append the message itself
#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
  writtenBytes = vsnprintf(currentPointer, sizeLeft, fmt, args) + 1;
#ifdef __clang__
#pragma GCC diagnostic pop
#endif

  if ((size_t) writtenBytes > sizeLeft)
    goto overflow_occured;
  // Get offset by subtracting originating array and the
  // pointer of its element
  record.uMessageOffset = currentPointer - threadBuffer;
  currentPointer += writtenBytes;
  sizeLeft -= (size_t) writtenBytes;
  
overflow_occured:;
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  printf("Line: [%ju] [<placeholder>/%s] [%s:%d#%s] %s\n", (uintmax_t) time.tv_sec, loglevelToStr(loglevel), callSite->sourceFile, callSite->line, callSite->shortFuncName, threadBuffer);
}



