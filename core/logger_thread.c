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

static void* readerThread(void*) {
  fprintf(stderr, "Started\n");
  while (atomic_load(&wantShutdown) == false) {
    const flup_log_record* record = logger_read_log();
    fprintf(stderr, "[Logger thread] Source: %s\n", record->uSourcePath);
    fprintf(stderr, "[Logger thread] Func name: %s\n", record->uShortFuncName);
    fprintf(stderr, "[Logger thread] Message: %s\n", record->uMessage);
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


