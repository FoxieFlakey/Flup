#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "flup/concurrency/cond.h"
#include "flup/attributes.h"
#include "flup/bug.h"
#include "flup/concurrency/mutex.h"
#include "flup/core/panic.h"

FLUP_PUBLIC
flup_cond* flup_cond_new() {
  flup_cond* self = malloc(sizeof(*self));
  if (!self)
    goto failure;
  
  *self = (flup_cond) {};
  
  self->initialized = false;
  if (pthread_cond_init(&self->cond, NULL) != 0)
    goto failure;
  self->initialized = true;
  return self;

failure:
  flup_cond_free(self);
  return NULL;
}

FLUP_PUBLIC
void flup_cond_free(flup_cond* self) {
  if (!self)
    return;
  
  if (self->initialized) {
    int ret = pthread_cond_destroy(&self->cond);
    BUG_ON(ret != 0);
  }
  free(self);
}

FLUP_PUBLIC
void flup_cond_wake_one(flup_cond* self) {
  int ret = pthread_cond_signal(&self->cond);
  BUG_ON(ret != 0);
}

FLUP_PUBLIC
void flup_cond_wake_all(flup_cond* self) {
  int ret = pthread_cond_broadcast(&self->cond);
  BUG_ON(ret != 0);
}

FLUP_PUBLIC
int flup_cond_wait(flup_cond* self, flup_mutex* mutex, const struct timespec* abstimeout) {
  int ret;
  if (abstimeout) {
    ret = pthread_cond_timedwait(&self->cond, &mutex->lock, abstimeout);
    if (ret != 0 && ret != ETIMEDOUT)
      flup_panic("pthread_cond_timedwait failed with %d when it can only failed with -ETIMEDOUT", ret);
  } else {
    if ((ret = pthread_cond_wait(&self->cond, &mutex->lock)) != 0)
      flup_panic("pthread_cond_wait failed with %d when it can't failed!", ret);
  }
  
  return -ret;
}

