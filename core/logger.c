#include <assert.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <threads.h>
#include <unistd.h>

#include "flup/core/logger.h"
#include "flup/attributes.h"
#include "flup/concurrency/cond.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/buffer/circular_buffer.h"

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
atomic_bool flup_is_in_abort = false;
FLUP_PUBLIC_VAR
thread_local bool flup_is_thread_aborting = false;

FLUP_CIRCULAR_BUFFER_DEFINE_STATIC(buffer, flup_logbuffer, BUFFER_SIZE);
FLUP_MUTEX_DEFINE_STATIC(bufferLock);
FLUP_COND_DEFINE_STATIC(bufferEvent);

FLUP_PUBLIC
void flup__printk(const flup_printk_call_site_info* callSite, flup_loglevel loglevel, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  flup__vprintk(callSite, loglevel, fmt, args);
  va_end(args);
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
  size_t writtenBytes = 0;
  int currentWrittenBytes = 0;
  *currentPointer = '\0';
  
  // Append the source path if given
  if (!callSite->sourceFile)
    goto sourceNotGiven;
  
  currentWrittenBytes = snprintf(currentPointer, sizeLeft, "%s", callSite->sourceFile) + 1;
  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  // Get offset by subtracting originating array and the
  // pointer of its element
  record.uSourcePathOffset = currentPointer - threadBuffer;
  currentPointer += currentWrittenBytes;
  sizeLeft -= (size_t) currentWrittenBytes;
  writtenBytes += (size_t) currentWrittenBytes;
sourceNotGiven:
  
  // Append the func name if given
  if (!callSite->shortFuncName)
    goto funcNameNotGiven;
  
  currentWrittenBytes = snprintf(currentPointer, sizeLeft, "%s", callSite->shortFuncName) + 1;
  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  // Get offset by subtracting originating array and the
  // pointer of its element
  record.uShortFuncNameOffset = currentPointer - threadBuffer;
  currentPointer += currentWrittenBytes;
  sizeLeft -= (size_t) currentWrittenBytes;
  writtenBytes += (size_t) currentWrittenBytes;
funcNameNotGiven:
  
  // Append the message itself
#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
  currentWrittenBytes = vsnprintf(currentPointer, sizeLeft, fmt, args) + 1;
#ifdef __clang__
#pragma GCC diagnostic pop
#endif

  if ((size_t) currentWrittenBytes > sizeLeft)
    goto overflow_occured;
  // Get offset by subtracting originating array and the
  // pointer of its element
  record.uMessageOffset = currentPointer - threadBuffer;
  currentPointer += currentWrittenBytes;
  sizeLeft -= (size_t) currentWrittenBytes;
  writtenBytes += (size_t) currentWrittenBytes;
  
overflow_occured:
  // Set record size
  record.recordSize = sizeof(record) + writtenBytes;
  
  // Write to the log buffer
  flup_mutex_lock(&bufferLock);
  size_t totalSize = sizeof(record) + writtenBytes;
  
  // Wait until there space to write
  while (buffer.bufferSize - buffer.usedSize < totalSize)
    flup_cond_wait(&bufferEvent, &bufferLock, NULL);
  
  int ret = flup_circular_buffer_write(&buffer, &record, sizeof(record));
  assert(ret == 0);
  ret = flup_circular_buffer_write(&buffer, threadBuffer, writtenBytes);
  assert(ret == 0);
  flup_mutex_unlock(&bufferLock);
}

const flup_log_record* logger_read_log() {
  static thread_local char threadBuffer[THREAD_BUFFER_SIZE];
  static thread_local flup_log_record record;
  flup_mutex_lock(&bufferLock);
  
  // Wait until there enough data to read 
  // whole record header
  while (buffer.usedSize < sizeof(record))
    flup_cond_wait(&bufferEvent, &bufferLock, NULL);
  
  // Read the log header
  int ret = flup_circular_buffer_read(&buffer, &record, sizeof(record));
  assert(ret == 0);
  
  size_t stringSize = record.recordSize - sizeof(record);
  
  // Wait until there enough data to read
  // all the strings 
  while (buffer.usedSize < stringSize)
    flup_cond_wait(&bufferEvent, &bufferLock, NULL);
  
  // Then read the strings
  ret = flup_circular_buffer_read(&buffer, threadBuffer, record.recordSize - sizeof(record));
  assert(ret == 0);
  flup_mutex_unlock(&bufferLock);
  
  // Convert the offsets into pointer
  record.uSourcePath = &threadBuffer[record.uSourcePathOffset];
  record.uShortFuncName = &threadBuffer[record.uShortFuncNameOffset];
  record.uMessage = &threadBuffer[record.uMessageOffset];
  
  return &record;
}


