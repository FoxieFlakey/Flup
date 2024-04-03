#include <stddef.h>
#include <pthread.h>
#include <Block.h>
#include <stdlib.h>
#include <string.h>

#include "flup/attributes.h"
#include "flup/compiler.h"
#include "flup/thread/thread.h"

#if FLUP_APPLE_BLOCKS_AVAILABLE
static void releaseBlock(void* udata) {
  flup_thread_block block = udata;
  Block_release(block);
}

static void blockTrampoline(void* udata) {
  flup_thread_block block = udata;
  pthread_cleanup_push(releaseBlock, udata);
  block();
  pthread_cleanup_pop(1);
}

FLUP_PUBLIC
flup_thread* flup_thread_new_with_block(flup_thread_block block) {
  flup_thread_block blockCopy = Block_copy(block);
  if (!blockCopy)
    return NULL;
  return flup_thread_new(blockTrampoline,blockCopy);
}
#endif

struct exec_data {
  flup_thread_func func;
  void* udata;
  flup_thread* thread;
};

[[noreturn]]
static void* executorFunc(void* data) {
  struct exec_data execData = *((struct exec_data*) data);
  // Exec data was allocated from flup_thread_new
  free(data);
  execData.func(execData.udata);
  pthread_exit(NULL);
}

FLUP_PUBLIC
flup_thread* flup_thread_new(flup_thread_func func, void* udata) {
  flup_thread* self = malloc(sizeof(*self));
  if (!self)
    return NULL;
  
  *self = (flup_thread) {
    .name = strdup("FlupThread-Unnamed")
  }; 
  
  if (!self->name)
    goto failure;
  
  if (pthread_attr_init(&self->attribute) != 0)
    goto failure;
  self->attributeInitialized = true;
  
  struct exec_data* execData = malloc(sizeof(*execData));
  if (!execData)
    goto failure;
  
  *execData = (struct exec_data) {
    .func = func,
    .udata = udata,
    .thread = self
  };
  
  if (pthread_create(&self->thread, &self->attribute, executorFunc, execData) != 0)
    goto failure;
  self->threadInitialized = true;
  return self;
  
failure:
  flup_thread_free(self);
  return NULL;
}

FLUP_PUBLIC
void flup_thread_free(flup_thread* self) {
  if (!self)
    return;
  
  if (self->attributeInitialized)
    pthread_attr_destroy(&self->attribute);
  free((char*) self->name);
  free(self);
}

FLUP_PUBLIC
void flup_thread_wait(flup_thread* self) {
  pthread_join(self->thread, NULL);
}
