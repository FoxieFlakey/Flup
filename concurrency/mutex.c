#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>

#include "flup/attributes.h"
#include "flup/bug.h"
#include "flup/concurrency/mutex.h"

FLUP_PUBLIC
flup_mutex* flup_mutex_new() {
  flup_mutex* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  self->initialized = false;
  
  if (pthread_mutex_init(&self->lock, NULL) != 0) {
    free(self);
    return NULL;
  }
  
  self->initialized = true;
  return self;
}

FLUP_PUBLIC
void flup_mutex_free(flup_mutex* self) {
  if (!self)
    return;
    
  if (self->initialized) {
    int ret = pthread_mutex_destroy(&self->lock);
    BUG_ON(ret != 0);
  }
  free(self);
}

FLUP_PUBLIC
void flup_mutex_lock(flup_mutex* self) {
  int ret = pthread_mutex_lock(&self->lock);
  BUG_ON(ret != 0);
}

FLUP_PUBLIC
void flup_mutex_unlock(flup_mutex* self) {
  int ret = pthread_mutex_unlock(&self->lock);
  BUG_ON(ret != 0);
}

