#include <pthread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>

#include "flup/attributes.h"
#include "flup/concurrency/mutex.h"

FLUP_PUBLIC
flup_mutex* flup_mutex_new() {
  flup_mutex* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  self->initialized = false;
  
  if (pthread_mutex_init(&self->lock, NULL) != 0) {
    flup_mutex_free(self);
    return NULL;
  }
  
  self->initialized = true;
  atomic_init(&self->owner, NULL);
  return self;
}

FLUP_PUBLIC
void flup_mutex_free(flup_mutex* self) {
  pthread_mutex_destroy(&self->lock);
  free(self);
}

FLUP_PUBLIC
void flup_mutex_lock(flup_mutex* self) {
  pthread_mutex_lock(&self->lock);
}

FLUP_PUBLIC
void flup_mutex_unlock(flup_mutex* self) {
  pthread_mutex_unlock(&self->lock);
}

