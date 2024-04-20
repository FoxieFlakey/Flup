#include "flup/bug.h"
#include "flup/attributes.h"
#include "flup/core/panic.h"

[[noreturn]]
FLUP_PUBLIC
void flup__trigger_bug(const char* file, int line, const char* func) {
  if (!file)
    file = "<unknown>";
  if (line < 0)
    line = -1;
  if (!func)
    func = "<unknown>";
  flup_panic("BUG: failure at %s:%d/%s()!", file, line, func);
}

