#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/uio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <threads.h>
#include <unistd.h>
#include <errno.h>

#include "flup/core/logger.h"
#include "core/logger_thread.h"
#include "flup/attributes.h"
#include "flup/concurrency/cond.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/buffer/circular_buffer.h"

#include "flup/thread/thread.h"
#include "logger.h"

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
atomic_bool flup_is_aborting = false;
FLUP_PUBLIC_VAR
thread_local bool flup_is_current_thread_aborting = false;

static atomic_bool loggerThreadIsStarting = false;

FLUP_CIRCULAR_BUFFER_DEFINE_STATIC(buffer, flup_logbuffer, BUFFER_SIZE);
FLUP_MUTEX_DEFINE_STATIC(bufferLock);
FLUP_COND_DEFINE_STATIC(dataWrittenBufferEvent);
FLUP_COND_DEFINE_STATIC(dataReadBufferEvent);

FLUP_PUBLIC
void flup__printk(const printk_call_site_info* callSite, const char* category, flup_loglevel loglevel, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flup__vprintk(callSite, category, loglevel, fmt, args);
  va_end(args);
}

FLUP_PUBLIC
void flup__vprintk(const printk_call_site_info* callSite, const char* category, flup_loglevel loglevel, const char* fmt, va_list args) {
  // If false that mean this is the winning thread attempting
  // to start logger thread, the rest will write to buffer
  if (!atomic_load(&logger_thread_has_started))
    if (atomic_exchange(&loggerThreadIsStarting, true) == false)
      logger_thread_start();
  
  static thread_local char threadBuffer[THREAD_BUFFER_SIZE];
  flup_log_record record = {
    .logLevel = loglevel,
    .funcPtr = callSite->funcPtr,
    .line = callSite->line,
    .uShortFuncNameOffset = -1,
    .uSourcePathOffset = -1,
    .uMessageOffset = -1,
    .uCategoryOffset = -1,
    .uThreadNameOffset = -1
  };
  
  if (clock_gettime(CLOCK_REALTIME, &record.timestamp) != 0) {
    // On error default to epoch
    record.timestamp.tv_nsec = 0;
    record.timestamp.tv_nsec = 0;
  }
  
  size_t sizeLeft = sizeof(threadBuffer);
  char* currentPointer = threadBuffer;
  size_t writtenBytes = 0;
  int currentWrittenBytes = 0;
  *currentPointer = '\0';
  
  // Get offset by subtracting originating array and the
  // pointer of its element
# define storeOffsetAndIncrement(field) do {\
    record.field = currentPointer - threadBuffer; \
    currentPointer += currentWrittenBytes; \
    sizeLeft -= (size_t) currentWrittenBytes; \
    writtenBytes += (size_t) currentWrittenBytes; \
  } while (0)
  
  // Append the source path if given
  if (!callSite->sourceFile)
    goto sourceNotGiven;
  currentWrittenBytes = snprintf(currentPointer, sizeLeft, "%s", callSite->sourceFile) + 1;
  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  storeOffsetAndIncrement(uSourcePathOffset);
sourceNotGiven:
  
  // Append the func name if given
  if (!callSite->shortFuncName)
    goto funcNameNotGiven;
  currentWrittenBytes = snprintf(currentPointer, sizeLeft, "%s", callSite->shortFuncName) + 1;
  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  storeOffsetAndIncrement(uShortFuncNameOffset);
funcNameNotGiven:
  
  // Append the category if given
  if (!category)
    goto categoryNotGiven;
  currentWrittenBytes = snprintf(currentPointer, sizeLeft, "%s", category) + 1;
  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  storeOffsetAndIncrement(uCategoryOffset);
categoryNotGiven:
  
  // Append current thread name if given
  if (!flup_current_thread || !flup_current_thread->name)
    goto threadNameUnavailable;
  currentWrittenBytes = snprintf(currentPointer, sizeLeft, "%s", flup_current_thread->name) + 1;
  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  storeOffsetAndIncrement(uThreadNameOffset);
threadNameUnavailable:
  
  // Append the message itself
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
  currentWrittenBytes = vsnprintf(currentPointer, sizeLeft, fmt, args) + 1;
  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  storeOffsetAndIncrement(uMessageOffset);
#ifdef __clang__
#pragma clang diagnostic pop
#endif

# undef storeOffsetAndIncrement
overflow_occured:
  // Set record size
  record.recordSize = sizeof(record) + writtenBytes;
  
  // Write to the log buffer
  flup_mutex_lock(&bufferLock);
  
  // Wait until there space to write whole record plus strings
  while (buffer.bufferSize - buffer.usedSize < record.recordSize)
    flup_cond_wait(&dataReadBufferEvent, &bufferLock, NULL);
  
  flup_circular_buffer_write(&buffer, &record, sizeof(record));
  flup_circular_buffer_write(&buffer, threadBuffer, writtenBytes);
  
  flup_cond_wake_one(&dataWrittenBufferEvent);
  flup_mutex_unlock(&bufferLock);
}

const flup_log_record* logger_read_log() {
  static thread_local char threadBuffer[THREAD_BUFFER_SIZE];
  static thread_local flup_log_record record;
  
  flup_mutex_lock(&bufferLock);
  // Wait until there enough data to read 
  // whole record header
  while (buffer.usedSize < sizeof(record))
    flup_cond_wait(&dataWrittenBufferEvent, &bufferLock, NULL);
  
  // Read the log header
  flup_circular_buffer_read(&buffer, &record, sizeof(record));
  flup_cond_wake_one(&dataReadBufferEvent);
  
  size_t stringSize = record.recordSize - sizeof(record);
  
  // Wait until there enough data to read
  // all the strings 
  while (buffer.usedSize < stringSize)
    flup_cond_wait(&dataWrittenBufferEvent, &bufferLock, NULL);
  
  // Then read the strings
  flup_circular_buffer_read(&buffer, threadBuffer, record.recordSize - sizeof(record));
  flup_cond_wake_one(&dataReadBufferEvent);
  flup_mutex_unlock(&bufferLock);
  
  // Convert the offsets into pointer
# define initField(field) do { \
    if (record.field ## Offset >= 0) \
      record.field = &threadBuffer[record.field ## Offset]; \
    else \
      record.field = NULL; \
  } while (0)
  
  initField(uSourcePath);
  initField(uShortFuncName);
  initField(uCategory);
  initField(uThreadName);
  initField(uMessage);
#undef initField
  
  return &record;
}

FLUP_PUBLIC
int flup_flush_logs(const struct timespec* abstimeout) {
  int ret = 0;
  flup_mutex_lock(&bufferLock);
  
  // Wait until the buffer is empty
  while (buffer.usedSize > 0) {
    if (flup_cond_wait(&dataReadBufferEvent, &bufferLock, abstimeout) == -ETIMEDOUT) {
      ret = -ETIMEDOUT;
      goto log_is_flushed;
    }
  }

log_is_flushed:
  flup_mutex_unlock(&bufferLock);
  return ret;
}

