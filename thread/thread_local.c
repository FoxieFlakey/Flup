#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

#include "flup/thread/thread_local.h"
#include "flup/attributes.h"

FLUP_PUBLIC
flup_thread_local* flup_thread_local_new(flup_thread_local_destructor destructor) {
  flup_thread_local* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_thread_local) {
    .initialized = false
  };
  
  // TODO: Find better way than casting function types
  static_assert(sizeof(uintptr_t) == sizeof(void*));
  if (pthread_key_create(&self->key, (void(*)(void*)) destructor) != 0) {
    free(self);
    return NULL;
  }
  
  self->initialized = true;
  return self;
}

FLUP_PUBLIC
void flup_thread_local_free(flup_thread_local* self) {
  if (!self)
    return;
  
  if (self->initialized)
    pthread_key_delete(self->key);
  free(self);
}

FLUP_PUBLIC
void flup_thread_local_set(flup_thread_local* self, uintptr_t newValue) {
  pthread_setspecific(self->key, (void*) newValue);
}

FLUP_PUBLIC
uintptr_t flup_thread_local_get(flup_thread_local* self) {
  return (uintptr_t) pthread_getspecific(self->key);
}

