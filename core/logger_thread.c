#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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
  fprintf(stderr, "Started\n");
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
    
    // Format is [timestamp] [subsystemName] [ThreadName/loglevel] [FileSource.c:line#function()] Message
    // Example: [Sat 12 Aug 2023, 10:31 AM +0700] [Renderer] [Render Thread/INFO] [renderer/renderer.c:20#init()] Initalizing OpenGL...
    fprintf(stderr, "[%s] [%s] [%s/%s] [%s:%d#%s()] %s\n", timestampBuffer, record->uCategory ? record->uCategory : "Uncategorized", record->uThreadName ? record->uThreadName : "<Unknown Thread>", loglevelToString(record->logLevel), record->uSourcePath, record->line, record->uShortFuncName, record->uMessage);
  }
  fprintf(stderr, "Exiting\n");
  pthread_exit(NULL);
}

static void onExit() {
  flup_printk(FLUP_FATAL, "Flushing logs... (not fatal but make sure its not filtered)");
  struct timespec timeout;
  clock_gettime(CLOCK_REALTIME, &timeout);
  timeout.tv_sec += 5;
  if (flup_flush_logs(&timeout) == -ETIMEDOUT)
    flup_printk(FLUP_FATAL, "Log did not flushed within timeout");
  
  atomic_store(&wantShutdown, true);
  flup_printk(FLUP_FATAL, "Stopping thread... (not fatal but make sure its not filtered and wakes the thread)");
  pthread_join(thread, NULL);
}

static void startup() {
  latestStartupAttempt = -pthread_create(&thread, NULL, readerThread, NULL);
  if (latestStartupAttempt != 0)
    return;
  
  atexit(onExit);
}

int logger_thread_start() {
  pthread_once(&tryStartOnce, startup);
  return latestStartupAttempt;
}


