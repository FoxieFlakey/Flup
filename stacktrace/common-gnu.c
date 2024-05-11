#define _GNU_SOURCE

#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <dlfcn.h>

#include "flup/stacktrace/stacktrace.h"

#include "common.h"

int stacktrace_get_object(uintptr_t ip, flup_stacktrace_object* res) {
  Dl_info info;
  if (!dladdr((void*) ip, &info))
    return -ENOENT;
  
  *res = (flup_stacktrace_object) {
    .objectBaseAddress = (uintptr_t) info.dli_fbase,
    .objectPath = info.dli_fname
  };
  
  return 0;
}

int stacktrace_get_source(uintptr_t, flup_stacktrace_source* res) {
  return -ENOSYS;
}

int stacktrace_get_symbol(uintptr_t ip, flup_stacktrace_symbol* res) {
  Dl_info info;
  if (!dladdr((void*) ip, &info))
    return -ENOENT;
  
  // If both unknown, that mean dladdr only knows that its
  // in an object which essentially useless for this function
  if (!info.dli_sname && !info.dli_saddr)
    return -ENOENT;
  
  *res = (flup_stacktrace_symbol) {
    .symbolName = info.dli_sname,
    .symbolAddr = (uintptr_t) info.dli_saddr
  };
  
  return 0;
}

