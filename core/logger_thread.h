#ifndef UWU_A770D258_05B6_427A_8498_346BD6B528E0_UWU
#define UWU_A770D258_05B6_427A_8498_346BD6B528E0_UWU

#include <stdatomic.h>

// Start the logger thread
// Only first call actually try to start, other calls returned
// the first call value unconditionally
int logger_thread_start();

extern atomic_bool logger_thread_has_started;

#endif
