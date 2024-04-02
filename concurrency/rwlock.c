#include <pthread.h>
#include <stdlib.h>

#include "flup/attributes.h"

#include "flup/concurrency/rwlock.h"

FLUP_PUBLIC
flup_rwlock* flup_rwlock_new() {
  flup_rwlock* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  self->initialized = false;
  
  if (pthread_rwlock_init(&self->rwlock, NULL) != 0) {
    flup_rwlock_free(self);
    return NULL;
  }
  
  self->initialized = true;
  return self;
}

FLUP_PUBLIC
void flup_rwlock_free(flup_rwlock* self) {
  if (!self)
    return;
  
  pthread_rwlock_destroy(&self->rwlock);
  free(self);
}

FLUP_PUBLIC
void flup_rwlock_rdlock(flup_rwlock* self) {
  pthread_rwlock_rdlock(&self->rwlock);
}

FLUP_PUBLIC
void flup_rwlock_wrlock(flup_rwlock* self) {
  pthread_rwlock_wrlock(&self->rwlock);
}

FLUP_PUBLIC
void flup_rwlock_unlock(flup_rwlock* self) {
  pthread_rwlock_unlock(&self->rwlock);
}
