#include <stdio.h>
#include <unistd.h>

#include "flup/thread/thread.h"
#include "flup/attributes.h"

#include "main.h"

FLUP_PUBLIC
int fluffedup_main(FLUP_UNUSED int argc, FLUP_UNUSED const char** argv) {
  fprintf(stderr, "[Parent] Starting thread\n");
  flup_thread* thrd = flup_thread_new_with_block(^() {
    fprintf(stderr, "[Child] Hello UwU\n");
    sleep(2);
    fprintf(stderr, "[Child] Terminating thread\n");
  });
  
  fprintf(stderr, "[Parent] Waiting thread\n");
  flup_thread_wait(thrd);
  
  fprintf(stderr, "[Parent] Cleaning thread\n");
  flup_thread_free(thrd);
  return 0;
}

