#ifndef UWU_D5D49FFC_4CF8_4DE3_8FA5_8FC2B91110EF_UWU
#define UWU_D5D49FFC_4CF8_4DE3_8FA5_8FC2B91110EF_UWU

#include <stdarg.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sys/types.h>
#include <time.h>

#include "flup/attributes.h"

/**
 * @file
 * @brief Flup's logging system
 *
 * Its uses a circular buffer to store the log entries while
 * waiting for reader thread to read it. This logger for now
 * wait for the readers.
 * 
 * The design of log system primarily aim to avoid I/O wait or
 * atleast absorbs the I/O wait impact and also when program
 * crash so early that no log saved. Users can read the log in
 * dump file.
 *
 * This design also largely inspired from Linux kernel
 */

/**
 * @brief Log levels
 *
 * The urgency of a level is counted from most urgent (0) to 
 * least urgent (8).
 */
typedef enum flup_loglevel {
  /// Unrecoverable situation (program cannot progress further)
  FLUP_FATAL      = 0,
  /// Need immediate action
  FLUP_ALERT      = 1,
  /// Critical event has occured
  FLUP_CRITICAL   = 2,
  /// An error occured
  FLUP_ERROR      = 3,
  /// Warning about something
  FLUP_WARN       = 4,
  /// Information which may be significant
  FLUP_NOTICE     = 5,
  /// Informational purpose doesn't tell much about error
  FLUP_INFO       = 6,
  /// Moar output
  FLUP_VERBOSE    = 7,
  /// EVEN MOAR OUTPUT UWU
  FLUP_DEBUG      = 8
} flup_loglevel;

/**
 * @brief Structure about @ref flup_printk call site
 *
 * Optionally by default used to track where @ref flup_printk
 * was called from to aid in more detailed log.
 */
typedef struct flup_printk_call_site_info {
  /**
   * @brief Pointer to function which called @ref flup_printk
   * 
   * Can be NULL if caller can't determinate and let
   * Flup try to fill this.
   */
  void* funcPtr;
  
  /**
   * @brief Name of the caller function
   *
   * This is filled using with __func__.
   *
   * Can be NULL if caller can't know the name and let
   * Flup try to fill this.
   */
  const char* shortFuncName;
  
  /**
   * @brief Path to source file of origin
   *
   * The value is taken from __FILE__ macro during the call 
   * to @ref flup_printk. The function will try fill this if
   * its NULL value or "Source.c" if it wasn't able to fill this.
   *
   * @note The file path pointed by this isn't always present
   */
  const char* sourceFile;
  
  /**
   * @brief Line number in source file of origin
   *
   * The value is taken from __LINE__ macro during the call
   * to @ref flup_printk. The function will try fill this if
   * its negative value or leave it at negative if it wasn't
   * able to fill this.
   */
  int line;
} flup_printk_call_site_info;

/**
 * @brief Symbol to log buffer
 *
 * @warning Its only used for debugging after death,
 *   the content format and structure won't be stable.
 *   Pass the content through 'strings' command and
 *   you'll get log messages jumbled with other stuffs.
 *   The char type does not imply its a printable string
 */
FLUP_PUBLIC_VAR
extern char flup_logbuffer[];

/// The size of logbuffer
FLUP_PUBLIC_VAR
extern size_t flup_logbuffer_size;

/// Is an abort in progress (panic, bug, etc)
FLUP_PUBLIC_VAR
extern atomic_bool flup_is_aborting;

/// Is current thread aborting?
FLUP_PUBLIC_VAR
extern thread_local bool flup_is_current_thread_aborting;

/// @cond
FLUP_PUBLIC
void flup__vprintk(const flup_printk_call_site_info* callSite, const char* category, flup_loglevel loglevel, const char* fmt, va_list args);

#if defined(__has_attribute) && __has_attribute(format)
__attribute__((format(printf, 4, 5)))
#endif
FLUP_PUBLIC
void flup__printk(const flup_printk_call_site_info* callSite, const char* category, flup_loglevel loglevel, const char* fmt, ...);
/// @endcond

/// Format fmt in a way to allow each source to have own format
#ifndef flup_fmt
# define flup_fmt(fmt) fmt
#endif

/**
 * @brief Write a log entry
 *
 * This must be called on its own statement
 *
 * @param loglevel The loglevel the entry is
 * @param fmt The printf format for log
 * @param ... The rest of arguments for printf format
 */
#define flup_printk(loglevel, fmt, ...) do {\
  static const flup_printk_call_site_info callSite = { \
    .shortFuncName = __func__, \
    .sourceFile = __FILE__, \
    .line = __LINE__, \
    .funcPtr = NULL \
  }; \
  flup__printk(&callSite, FLUP_LOG_CATEGORY, (loglevel), flup_fmt(fmt) __VA_OPT__(,) __VA_ARGS__); \
} while(0)

/// @{
/**
 * @brief Short wrapper around @ref flup_printk with coresponding level
 */
#define pr_fatal(fmt, ...) flup_printk(FLUP_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_alert(fmt, ...) flup_printk(FLUP_ALERT, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_critical(fmt, ...) flup_printk(FLUP_CRITICAL, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_error(fmt, ...) flup_printk(FLUP_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_warn(fmt, ...) flup_printk(FLUP_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_notice(fmt, ...) flup_printk(FLUP_NOTICE, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_info(fmt, ...) flup_printk(FLUP_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_verbose(fmt, ...) flup_printk(FLUP_VERBOSE, fmt __VA_OPT__(,) __VA_ARGS__)
#define pr_debug(fmt, ...) flup_printk(FLUP_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
/// @}

/**
 * @brief Write a log entry (va_list alternative)
 *
 * This must be called on its own statement
 *
 * @param loglevel The loglevel the entry is
 * @param fmt The printf format for log
 * @param args The rest of arguments for printf format
 */
#define flup_vprintk(loglevel, fmt, args) do {\
  static const flup_printk_call_site_info callSite = { \
    .shortFuncName = __func__, \
    .sourceFile = __FILE__, \
    .line = __LINE__, \
    .funcPtr = NULL \
  }; \
  flup__vprintk(&callSite, FLUP_LOG_CATEGORY, (loglevel), flup_fmt(fmt), args); \
} while(0)

/**
 * @brief Record for a single log entry
 */
typedef struct flup_log_record {
  /// The size of whole record including this structure
  size_t recordSize;
  /// Pointer to function (or NULL if can't be retrieved)
  void* funcPtr;
  /// Line number where the log was generated (or -1 if can't be retrieved)
  int line;
  /// Loglevel
  flup_loglevel logLevel;
  /// Time when this log generated
  struct timespec timestamp;
  
  /// Source where the log was generated or NULL or -1 if can't be retrieved
  union {
    /// Pointer to string itself
    const char* uSourcePath;
    /// In serialized form only, offset to the string
    ssize_t uSourcePathOffset;
  };
  
  /// Function name or NULL or -1 if can't be retrieved
  union {
    /// Pointer to string itself
    const char* uShortFuncName;
    /// In serialized form only, offset to the string
    ssize_t uShortFuncNameOffset;
  };
  
  /// Category (or subsystem) of the log origin
  union {
    /// Pointer to string itself
    const char* uCategory;
    /// In serialized form only, offset to the string
    ssize_t uCategoryOffset;
  };
  
  /// Thread of the log entry origin
  union {
    /// Pointer to string itself
    const char* uThreadName;
    /// In serialized form only, offset to the string
    ssize_t uThreadNameOffset;
  };
  
  /// Message in the log
  union {
    /// Pointer to string itself
    const char* uMessage;
    /// In serialized form only, offset to the string
    ssize_t uMessageOffset;
  };
  
  /// The string come after this struct
  char strings[];
} flup_log_record;

/**
 * @brief Wait until all logs processed
 *
 * @param abstimeout How long caller willing to wait or NULL if forever
 *
 * @return 0 on success or -ETIMEOUT if timeout reached (never returned
 *   if abstimeout is NULL)
 */
FLUP_PUBLIC
int flup_flush_logs(const struct timespec* abstimeout);

/**
 * @brief Category to be used to flup_pr_* functions
 *
 * Defaults to "Default" category
 *
 * This can be overrideable by each source by defining FLUP_LOG_CATEGORY
 * before including any Flup's headers 
 *
 * Its serve as easy way for defining category for a single source
 */
#ifndef FLUP_LOG_CATEGORY
# define FLUP_LOG_CATEGORY "Default"
#endif

#endif




