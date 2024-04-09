#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "flup/concurrency/cond.h"
#include "flup/attributes.h"
#include "flup/bug.h"
#include "flup/concurrency/mutex.h"

FLUP_PUBLIC
flup_cond* flup_cond_new() {
  flup_cond* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_cond) {};
  
  self->initialized = false;
  if (pthread_cond_init(&self->cond, NULL) != 0) {
    free(self);
    return NULL;
  }
  self->initialized = true;
  return self;
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
  if (abstimeout)
    ret = pthread_cond_timedwait(&self->cond, &mutex->lock, abstimeout);
  else
    ret = pthread_cond_wait(&self->cond, &mutex->lock);
  
  BUG_ON(ret != 0);
  return 0;
}

