#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "logger_thread.h"
#include "core/logger.h"
#include "flup/core/logger.h"

static int latestStartupAttempt;
static pthread_once_t tryStartOnce = PTHREAD_ONCE_INIT;
static pthread_t thread;
static atomic_bool wantShutdown = false;

atomic_bool logger_thread_has_started = false;

enum format {
  FORMAT_MINIMAL,
  FORMAT_SIMPLE,
  FORMAT_DETAILED
};
static atomic_int format = FORMAT_SIMPLE;

static const char* loglevelToString(flup_loglevel level) {
  switch (level) {
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

  return "UNKNOWN";
}

static void* readerThread(void*) {
  while (atomic_load(&wantShutdown) == false) {
    const flup_log_record* record = logger_read_log();
    
    struct tm brokenDownDateAndTime;
    
    // If there any error during localtime_r, default to epoch
    if (localtime_r(&record->timestamp.tv_sec, &brokenDownDateAndTime) == NULL) {
      time_t epoch = 0;
      localtime_r(&epoch, &brokenDownDateAndTime);
    }
    
    static char timestampBuffer[1024];
    strftime(timestampBuffer, sizeof(timestampBuffer), "%a %d %b %Y, %H:%M:%S %z", &brokenDownDateAndTime);
    
    switch (atomic_load(&format)) {
      case FORMAT_MINIMAL:
        // Format is [timestamp] [subsystemName] Message
        // Example: [Renderer] [INFO] Initalizing OpenGL...
        fprintf(stderr, "[%s] [%s] %s\n", record->uCategory ? record->uCategory : "Uncategorized", loglevelToString(record->logLevel), record->uMessage);
        break;
      case FORMAT_SIMPLE:
        // Format is [timestamp] [subsystemName] [loglevel] Message
        // Example: [Sat 12 Aug 2023, 10:31 AM +0700] [Renderer] [INFO] Initalizing OpenGL...
        fprintf(stderr, "[%s] [%s] [%s] %s\n", timestampBuffer, record->uCategory ? record->uCategory : "Uncategorized", loglevelToString(record->logLevel), record->uMessage);
        break;
      case FORMAT_DETAILED:
        // Format is [timestamp] [subsystemName] [ThreadName/loglevel] [FileSource.c:line#function()] Message
        // Example: [Sat 12 Aug 2023, 10:31 AM +0700] [Renderer] [Render Thread/INFO] [renderer/renderer.c:20#init()] Initalizing OpenGL...
        fprintf(stderr, "[%s] [%s] [%s/%s] [%s:%d#%s()] %s\n", timestampBuffer, record->uCategory ? record->uCategory : "Uncategorized", record->uThreadName ? record->uThreadName : "<Unknown Thread>", loglevelToString(record->logLevel), record->uSourcePath, record->line, record->uShortFuncName, record->uMessage);
        break;
    }
  }
  pthread_exit(NULL);
}

static void onExit() {
  pr_fatal("Flushing logs... (not fatal but make sure its not filtered, thread name is expected to be unknown due its already at the end of lifetime)");
  struct timespec timeout;
  clock_gettime(CLOCK_REALTIME, &timeout);
  timeout.tv_sec += 5;
  if (flup_flush_logs(&timeout) == -ETIMEDOUT)
    pr_fatal("Log did not flushed within timeout");
  
  atomic_store(&wantShutdown, true);
  pr_fatal("Stopping thread... (not fatal but make sure its not filtered and wakes the thread, thread name is expected to be unknown due its already at the end of lifetime)");
  pthread_join(thread, NULL);
}

static void startup() {
  const char* formatStringEnv = getenv("LIBFLUP_LOG_FORMAT");
  char formatStringBuffer[20];
  if (!formatStringEnv) {
    atomic_store(&format, FORMAT_SIMPLE);
    pr_info("LIBFLUP_LOG_FORMAT not set defaulting to 'simple'");
    goto log_format_not_defined;
  }
  
  strncpy(formatStringBuffer, formatStringEnv, sizeof(formatStringBuffer));
  if (strcmp(formatStringBuffer, "minimal") == 0) {
    atomic_store(&format, FORMAT_MINIMAL);
  } else if (strcmp(formatStringBuffer, "simple") == 0) {
    atomic_store(&format, FORMAT_SIMPLE);
  } else if (strcmp(formatStringBuffer, "detailed") == 0) {
    atomic_store(&format, FORMAT_DETAILED);
  } else {
    pr_error("Unknown 'LIBFLUP_LOG_FORMAT=%s' defaulting to 'simple'", formatStringBuffer);
    atomic_store(&format, FORMAT_SIMPLE);
  }

log_format_not_defined:
  latestStartupAttempt = -pthread_create(&thread, NULL, readerThread, NULL);
  if (latestStartupAttempt != 0)
    return;
  
  atexit(onExit);
  atomic_store(&logger_thread_has_started, true);
}

int logger_thread_start() {
  pthread_once(&tryStartOnce, startup);
  return latestStartupAttempt;
}


